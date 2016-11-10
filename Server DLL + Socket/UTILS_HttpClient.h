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
#ifndef _UTILS_HttpClient__h
#define _UTILS_HttpClient__h

#include <string.h>


#define UTILS_OK											0		/* "No Error" */
#define UTILS_ERR_HTTP_SEND									4501	/* "error sending http request" */
#define UTILS_ERR_HTTP_RECV									4502	/* "error receiving http response" */



class /* UTILS_IMP_EXP */ UTILS_HttpClient
{
	public:

												UTILS_HttpClient(const wchar_t * s_ServerName, int n_ServerPort);
												UTILS_HttpClient(const UTILS_HttpClient & _HttpClient);
	virtual										~UTILS_HttpClient();

			UTILS_HttpClient &					operator =(const UTILS_HttpClient &);

			const wchar_t *						getServerName() const { return m_s_ServerName; }
			int									getServerPort() const { return m_n_ServerPort; }

			void								setURLPath(const wchar_t * s_URLPath);
			const wchar_t *						getURLPath() const { return m_s_URLPath; }
			void								setIsSecure(bool b_IsSecure);
			bool								getIsSecure() const { return m_b_IsSecure; }
			void								setHeaders(const wchar_t * s_Headers);
			const wchar_t *						getHeaders() const { return m_s_Headers; }

			int									post(const wchar_t * s_request, wchar_t * & s_response);

  protected:

  private:

			wchar_t *							m_s_ServerName;
			int									m_n_ServerPort;
			wchar_t *							m_s_URLPath;
			bool								m_b_IsSecure;
			wchar_t *							m_s_Headers;

};


#endif /* _UTILS_HttpClient__h */
