// NPS_DLL_Example.cpp : Defines the exported functions for the DLL application.
//
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <Authif.h>
#include <stdio.h>

#include "NPS_DLL_Example.h"
#include <iostream>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/http_listener.h>              // HTTP server
#include <cpprest/json.h>                       // JSON library
#include <cpprest/uri.h>                        // URI library
#include <cpprest/containerstream.h>            // Async streams backed by STL containers
#include <cpprest/interopstream.h>              // Bridges for integrating Async streams with STL and WinRT streams
#include <cpprest/rawptrstream.h>               // Async streams backed by raw pointer to memory
#include <cpprest/producerconsumerstream.h>     // Async streams for producer consumer scenarios


using namespace std;
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace web::http::experimental::listener;          // HTTP server
using namespace web::json;                                  // JSON library

DWORD WINAPI RadiusExtensionProcess2(

	_Inout_  PRADIUS_EXTENSION_CONTROL_BLOCK pECB
	)
{
	if (pECB != NULL)
	{
		if (pECB->rcRequestType == rcAccessRequest) 
		{
			pECB->SetResponseType(pECB, rcAccessReject);

			RADIUS_ATTRIBUTE_ARRAY * pAR = pECB->GetRequest(pECB);
			if (pAR != NULL)
			{
				DWORD arSize = pAR->GetSize(pAR);
				for (DWORD iAR = 0; iAR < arSize; iAR++)
				{
					const RADIUS_ATTRIBUTE * pRA = pAR->AttributeAt(pAR, iAR);
					if (pRA != NULL)
					{
						if (pRA->dwAttrType == ratUserName)
						{
							
							const char * szUserName = (char *) pRA->lpValue;
							char buf[2048];
							sprintf_s(buf, "received request for: %s (type=%d)", szUserName, pRA->fDataType);
							LogMessage(buf);

							int sockResult = startSocket();

							if (sockResult != 0){
								char buf3[2048];
								sprintf_s(buf3, "NOES: (type=%d)", sockResult);
								LogMessage(buf3);
								pECB->SetResponseType(pECB, rcAccessAccept);
								
							}
							else{
								char buf4[2048];
								sprintf_s(buf4, "YES: (type=%d)", sockResult);
								LogMessage(buf4);
								pECB->SetResponseType(pECB, rcAccessReject);
							}
							
							return NO_ERROR;

							
							//startSocket();
					
						}
					}
				}
			}
		}
		else if (pECB->rcRequestType == rcAccountingRequest)
		{
			pECB->SetResponseType(pECB, rcDiscard);
		}
		else
		{
			pECB->SetResponseType(pECB, rcDiscard);
		}
	}

	//return NO_ERROR; // If the return value is anything other than NO_ERROR, NPS discards the request.
}


void LogMessage(const char * szMsg)
{
	FILE * fp = fopen("C:\\Temp\\t.txt", "a");
	if (fp)
	{
		fprintf(fp, "%s\n", szMsg);
		fclose(fp);
	}
}
