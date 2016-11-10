#include <winsock2.h>
#include <fstream>
#include "NPS_DLL_Example.h"

#include<stdio.h>
#include<winsock2.h>
#include<string>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/http_listener.h>              // HTTP server
#include <cpprest/json.h>                       // JSON library
#include <cpprest/uri.h>                        // URI library
#include <cpprest/containerstream.h>            // Async streams backed by STL containers
#include <cpprest/interopstream.h>              // Bridges for integrating Async streams with STL and WinRT streams
#include <cpprest/rawptrstream.h>               // Async streams backed by raw pointer to memory
#include "cpprest/containerstream.h"
#include "cpprest/filestream.h"
#include "cpprest/http_client.h"
#include "cpprest/json.h"
#include "cpprest/producerconsumerstream.h"
#include "cpprest/http_client.h"
#include "UTILS_HttpClient.h"
using namespace std;
using namespace web;
using namespace web::json;
using namespace web::http;
using namespace web::http::client;
using namespace utility;
using namespace utility::conversions;

//WinSock: Initializing

//	The first step is to call WSAStartup to startup the interface to WinSock.
int startSocket(){

	long WSAResult;
	WSADATA wsaData;
	WORD version;

	version = MAKEWORD(2, 2);

	//Start the Winsock DLL
	WSAResult = WSAStartup(version, &wsaData);

	//Creates the socket structure
	struct sockaddr_in address;
	int AddressSize = sizeof(address);

	//Creating the sockets
	SOCKET sock_listening;

	sock_listening = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_family = AF_INET;
	address.sin_port = htons(24111);
	
	if (bind(sock_listening, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	bind(sock_listening, (SOCKADDR *)&address, sizeof(address));

	long successMsg;
	for (;;){
		ofstream logFile;
		logFile.open("C:\\Temp\\connection.txt");
		logFile << "Connection is ready!\n";
		if (sock_listening == accept(sock_listening, (SOCKADDR*)&address, &AddressSize)){
			logFile << "Connection has been found!\n";

			successMsg = send(sock_listening, "Welcome, connection has been made", 46, NULL);
		}

		logFile.close();
	}
}


void LogMessage(const char * szMsg)
{
	FILE * fp = fopen("C:\\Temp\\connection.txt", "a");
	if (fp)
	{
		fprintf(fp, "%s\n", szMsg);
		fclose(fp);
	}
}