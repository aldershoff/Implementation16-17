/************************************************************************/
/*                                                                      */
/* author: Arjen Jansen                                                 */
/*         Jeanfils B.V.                                                */
/*         arjen.jansen@kpnmail.nl                                      */
/*                                                                      */
/* This Software is owned by Jeanfils B.V. and is protected             */
/* by copyright law and international copyright treaty. Therefore,      */
/* you must treat this Software like anyother copyrighted material      */
/* (e.g., a book).                                                      */
/*                                                                      */
/************************************************************************/

#include <windows.h>

#include <winhttp.h>
#include <stdio.h>
#include <string.h>
#include "UTILS_HttpClient.h"


UTILS_HttpClient::UTILS_HttpClient(const wchar_t * s_ServerName, int n_ServerPort)
{
	m_s_ServerName	= (s_ServerName) ? _wcsdup(s_ServerName) : 0;
	m_n_ServerPort	= n_ServerPort;
	m_s_URLPath		= 0;
	m_s_Headers		= 0;
	m_b_IsSecure	= false;
}


UTILS_HttpClient::UTILS_HttpClient(const UTILS_HttpClient & _HttpClient)
{
	m_s_ServerName	= (_HttpClient.m_s_ServerName) ? _wcsdup(_HttpClient.m_s_ServerName) : 0;
	m_n_ServerPort	= _HttpClient.m_n_ServerPort;
	m_s_URLPath		= (_HttpClient.m_s_URLPath) ? _wcsdup(_HttpClient.m_s_URLPath) : 0;
	m_s_Headers		= (_HttpClient.m_s_Headers) ? _wcsdup(_HttpClient.m_s_Headers) : 0;
	m_b_IsSecure	= _HttpClient.m_b_IsSecure;
}


UTILS_HttpClient::~UTILS_HttpClient()
{
	if (m_s_ServerName)
	{
		delete [] m_s_ServerName;
	}
	if (m_s_URLPath)
	{
		delete [] m_s_URLPath;
	}
	if (m_s_Headers)
	{
		delete [] m_s_Headers;
	}
}



UTILS_HttpClient & UTILS_HttpClient::operator =(const UTILS_HttpClient & _HttpClient)
{
	m_s_ServerName = (_HttpClient.m_s_ServerName) ? _wcsdup(_HttpClient.m_s_ServerName) : 0;
	m_n_ServerPort = _HttpClient.m_n_ServerPort;
	m_s_URLPath		= (_HttpClient.m_s_URLPath) ? _wcsdup(_HttpClient.m_s_URLPath) : 0;
	m_s_Headers		= (_HttpClient.m_s_Headers) ? _wcsdup(_HttpClient.m_s_Headers) : 0;
	m_b_IsSecure	= _HttpClient.m_b_IsSecure;

	return * this;
}


void UTILS_HttpClient::setURLPath(const wchar_t * s_URLPath)
{
	m_s_URLPath = (s_URLPath) ? _wcsdup(s_URLPath) : 0;
}


void UTILS_HttpClient::setIsSecure(bool b_IsSecure)
{
	m_b_IsSecure = b_IsSecure;
}


void UTILS_HttpClient::setHeaders(const wchar_t * s_Headers)
{
	m_s_Headers = (s_Headers) ? _wcsdup(s_Headers) : 0;
}


int UTILS_HttpClient::post(const wchar_t * s_request, wchar_t * & s_response)
{
	int rc = UTILS_OK;

	BOOL		bResults	= FALSE;
	HINTERNET	hSession	= NULL;
	HINTERNET	hConnect	= NULL;
	HINTERNET	hRequest	= NULL;

	hSession = WinHttpOpen( L"UTILS_HttpClient",
							WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, //WINHTTP_ACCESS_TYPE_NO_PROXY,
							WINHTTP_NO_PROXY_NAME,
							WINHTTP_NO_PROXY_BYPASS,
							0
								);

	if (hSession)
	{
		hConnect = WinHttpConnect(
							hSession,						// _In_        HINTERNET hSession
							m_s_ServerName,					// _In_        LPCWSTR pswzServerName,
							m_n_ServerPort,					// _In_        INTERNET_PORT nServerPort, INTERNET_DEFAULT_HTTPS_PORT|INTERNET_DEFAULT_HTTP_PORT
							0								// _Reserved_  DWORD dwReserved
								);
	}
	else
	{
		DWORD le = GetLastError();
		LPVOID lpMsgBuf;
		FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
						NULL,
						le,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
						(LPTSTR) &lpMsgBuf,
						0,
						NULL);
		fprintf(stderr, "UTILS_HttpClient::send() -> Error %u in WinHttpOpen()\n%s", le, (char *) lpMsgBuf);

		rc = UTILS_ERR_HTTP_SEND;
		LocalFree(lpMsgBuf);
	}

	if (hConnect)
	{
		hRequest = WinHttpOpenRequest(
							hConnect,						// _In_  HINTERNET hConnect,
							L"POST",						// _In_  LPCWSTR pwszVerb,
							(wchar_t *) m_s_URLPath,		// _In_  LPCWSTR pwszObjectName,
							NULL,							// _In_  LPCWSTR pwszVersion,
							WINHTTP_NO_REFERER,				// _In_  LPCWSTR pwszReferrer,
							WINHTTP_DEFAULT_ACCEPT_TYPES,	// _In_  LPCWSTR *ppwszAcceptTypes,
							WINHTTP_FLAG_BYPASS_PROXY_CACHE | (getIsSecure() ? WINHTTP_FLAG_SECURE : 0)	// _In_  DWORD dwFlags	WINHTTP_FLAG_SECURE
								);
	}
	else
	{
		DWORD le = GetLastError();
		LPVOID lpMsgBuf;
		FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
						NULL,
						le,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
						(LPTSTR) &lpMsgBuf,
						0,
						NULL);
		fprintf(stderr, "UTILS_HttpClient::send() -> Error %u in WinHttpConnect()\n%s", le, (char *) lpMsgBuf);

		rc = UTILS_ERR_HTTP_SEND;
		LocalFree(lpMsgBuf);
	}

	if (hRequest)
	{
		int dataSize = 0;
		if (s_request)
		{
			dataSize = wcslen(s_request);
		}

		bResults = WinHttpSendRequest(
							hRequest,					// _In_      HINTERNET hRequest,
							(wchar_t *) (m_s_Headers) ? (wchar_t *) m_s_Headers : WINHTTP_NO_ADDITIONAL_HEADERS,	// _In_opt_  LPCWSTR pwszHeaders,
							(m_s_Headers) ? -1L : 0,	// _In_      DWORD dwHeadersLength,
							(LPVOID) s_request,			// _In_opt_  LPVOID lpOptional,
							dataSize,					// _In_      DWORD dwOptionalLength,
							dataSize,					// _In_      DWORD dwTotalLength,
							NULL						// _In_      DWORD_PTR dwContext
								);
	}
	else
	{
		DWORD le = GetLastError();
		LPVOID lpMsgBuf;
		FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
						NULL,
						le,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
						(LPTSTR) &lpMsgBuf,
						0,
						NULL);
		fprintf(stderr, "UTILS_HttpClient::send() -> Error %u in WinHttpOpenRequest()\n%s", le, (char *) lpMsgBuf);

		rc = UTILS_ERR_HTTP_SEND;
		LocalFree(lpMsgBuf);
	}

	if (bResults)
	{
		bResults = WinHttpReceiveResponse(
							hRequest,
							NULL
								);
	}
	else
	{
		DWORD le = GetLastError();
		LPVOID lpMsgBuf;
		FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
						NULL,
						le,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
						(LPTSTR) &lpMsgBuf,
						0,
						NULL);
		fprintf(stderr, "UTILS_HttpClient::send() -> Error %u in WinHttpSendRequest()\n%s", le, (char *) lpMsgBuf);

		rc = UTILS_ERR_HTTP_SEND;
		LocalFree(lpMsgBuf);
	}

	LPSTR	recvStream	= 0;
	if (bResults)
	{
		DWORD		dwSize			= 0;
		DWORD		dwDownloaded	= 0;
		DWORD		dwTotalRead		= 0;
		do
		{
			dwSize = 0;
			if (WinHttpQueryDataAvailable(hRequest, & dwSize))
			{
				LPSTR pstrBuffer = new char[dwSize+1];
				if (! WinHttpReadData(hRequest, (LPVOID) pstrBuffer, dwSize, &dwDownloaded))
				{
					DWORD le = GetLastError();
					LPVOID lpMsgBuf;
					FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
									NULL,
									le,
									MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
									(LPTSTR) &lpMsgBuf,
									0,
									NULL);
					fprintf(stderr, "UTILS_HttpClient::send() -> Error %u in WinHttpReadData()\n%s", le, (char *) lpMsgBuf);

					rc = UTILS_ERR_HTTP_RECV;
					LocalFree(lpMsgBuf);
				}
				else
				{
					if (dwDownloaded > 0)
					{
						dwTotalRead += dwDownloaded;
						LPSTR pstrTotal = new char[dwTotalRead + 1];
						if (recvStream)
						{
							memcpy(pstrTotal, recvStream, dwTotalRead - dwDownloaded);
							delete [] recvStream;
						}
						memcpy(pstrTotal + dwTotalRead - dwDownloaded, pstrBuffer, dwDownloaded);
						pstrTotal[dwTotalRead] = '\0';

						recvStream = pstrTotal;
					}
				}
				delete [] pstrBuffer;
			}
			else
			{
				DWORD le = GetLastError();
				LPVOID lpMsgBuf;
				FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
								NULL,
								le,
								MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
								(LPTSTR) &lpMsgBuf,
								0,
								NULL);
				fprintf(stderr, "UTILS_HttpClient::send() -> Error %u in WinHttpQueryDataAvailable()\n%s", le, (char *) lpMsgBuf);

				rc = UTILS_ERR_HTTP_RECV;
				LocalFree(lpMsgBuf);
			}
		} while (rc == UTILS_OK && dwSize > 0 && dwTotalRead < dwSize);
		if (rc == UTILS_OK)
		{
			if (dwTotalRead)
			{
				wchar_t * recvStream_WC = new wchar_t[dwTotalRead * 4];
				int recvStream_Len = MultiByteToWideChar(CP_ACP, 0, recvStream, -1, recvStream_WC, dwTotalRead * 4);
				s_response = new wchar_t[recvStream_Len];
				wcscpy(s_response, recvStream_WC);
				delete [] recvStream_WC;
			}
			else
			{
				s_response = 0;
			}
		}
	}
	else
	{
		DWORD le = GetLastError();
		LPVOID lpMsgBuf;
		FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
						NULL,
						le,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
						(LPTSTR) &lpMsgBuf,
						0,
						NULL);
		fprintf(stderr, "UTILS_HttpClient::send() -> Error %u in WinHttpReceiveResponse()\n%s", le, (char *) lpMsgBuf);

		rc = UTILS_ERR_HTTP_RECV;
		LocalFree(lpMsgBuf);
	}

	if (hRequest)
	{
		WinHttpCloseHandle(hRequest);
	}
	if (hConnect)
	{
		WinHttpCloseHandle(hConnect);
	}
	if (hSession)
	{
		WinHttpCloseHandle(hSession);
	}

	return rc;
}
