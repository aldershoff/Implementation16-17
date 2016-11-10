// GCMSocketPOST3.cpp : Defines the entry point for the console application.
//



/*
TCP Echo server example in winsock
Live Server on port 8888
*/
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

//
#include"stdafx.h"
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
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace web::http::experimental::listener;          // HTTP server
using namespace web::json;                                  // JSON library


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "10000"

#pragma comment(lib, "ws2_32.lib") //Winsock Library
void sendPostMessage(json::value, json::value);
std::string random_str(std::size_t len);

int startSocket()
{

	// Variables for messages
	std::string MESSAGETOPOST;
	const char* messageToClient;

	// Variables for socket
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;




	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 2;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 3;
	}

	

		// Setup the TCP listening socket
		iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			printf("bind failed with error: %d\n", WSAGetLastError());
			freeaddrinfo(result);
			closesocket(ListenSocket);
			WSACleanup();
			return 4;
		
	}

	freeaddrinfo(result);

	/*
	Get the regID from AD to send to the GCM
	*/
	MESSAGETOPOST = "TEST";

	// Setting up the json Strings
	json::value gcmRegID = json::value::string(L"APA91bHjCCw9_vFkqrZJTgg9CE7VwEz2hZN0xz8EkDlDPnkpUsVkrETqwf_xz3x2GlCr6bQSo5b0_6pm6a0nqCSPsILDBYiYZmCQkYmRRwnv5IboOOxYfoe1GIuNdzwJKL-1AFVrHojv2eQ3pWCTWkKTh_wJ2qkA0Q");

	// Send regID for mobile authentication (socket need to know which mobile has authenticated back)
	json::value gcmMessage = json::value::string(L"TEST");

	sendPostMessage(gcmRegID, gcmMessage);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 5;
	}

	std::cout << "Socket is listening\n";

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 6;
	}

	std::cout << "Accepted connection\n";

	// No longer need server socket
	closesocket(ListenSocket);


	// Receive until the peer shuts down the connection
	do {

		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);

			//add null character, if you want to use with printf/puts or other string handling functions
			recvbuf[iResult] = '\0';

			// Do all the work here for replying
			const char* reply = recvbuf;

			// To String for iterating through array
			std::string replyString = std::string(reply, strlen(reply));

			std::cout << ("Message received: " + replyString + "\n");

			if (replyString.compare(MESSAGETOPOST) == 0){
				messageToClient = "OK";
				
			}
			else if (replyString.compare("CANCEL") == 0){
				messageToClient = "CANCELOK";
				iSendResult = send(ClientSocket, messageToClient, strlen(messageToClient), 0);
				closesocket(ClientSocket);
				WSACleanup();
				return 11;
			}
			else{
				messageToClient = "ERROR";
				// Echo the buffer back to the sender
				iSendResult = send(ClientSocket, messageToClient, strlen(messageToClient), 0);
				closesocket(ClientSocket);
				WSACleanup();
				return 7;
			}



			// Echo the buffer back to the sender
			iSendResult = send(ClientSocket, messageToClient, strlen(messageToClient), 0);
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 8;
			}
			printf("Message send: %d\n", messageToClient);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else  {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 9;
		}

	} while (iResult > 0);

	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 10;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();

	return 0;

}





void sendPostMessage(json::value jregID, json::value jmessage){

	// Set the client
	http_client client(L"https://android.googleapis.com/gcm/send");

	// Set the method to POST
	http_request request(methods::POST);

	// Add the headers for the GCM
	request.headers().add(L"Content-Type", L"application/json");
	request.headers().add(L"Authorization", L"key=AIzaSyBkFbdB7QLS5xiqe6kgLGhcbZN1PRTbg4c");

	// Setting the regID
	json::value mainData;
	json::value payLoad;

	mainData[L"collapse_key"] = json::value::string(L"message");

	payLoad[L"message"] = jmessage;

	// Setting the data array for the (title) and message
	mainData[L"data"] = payLoad;
	mainData[L"registration_ids"] = json::value::array({ jregID, json::value::string(L"APA91bFZ2m8II8yd2aCfgLOujgaeYmRjL8pBSdlcHGD59x4bf0Q_CdtEvR1QnCRAzNsU8Ptnj0bQgZEkJ8YCTgCvXsNfbV4awXR5KgDVDG37uyUijnnWDKRrQlUeeRAyC7NAQf1OWY53rgnHe6qBzXxVPOzZTkddOA") });


	// Put it into the body, with a serialization
	request.set_body(mainData.serialize());

	// Send the request to the GCM server and get response
	client.request(request).then([](http_response response)
	{
		// Print the status code.
		std::wostringstream ss;
		ss << L"Server returned returned status code " << response.status_code() << L"." << std::endl;

		std::wcout << ss.str();
	});
}