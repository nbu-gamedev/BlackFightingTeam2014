/*
 * ServerComponents.h
 *
 *  Created on: 22.11.2014 ã.
 *      Author: NewKnight
 */

#ifndef HEADERS_SERVERCOMPONENTS_H_
#define HEADERS_SERVERCOMPONENTS_H_

#include "Common/BSD.h"

#include <stdio.h>
#ifdef WIN32			//WIndows
#include <winsock2.h>
#include <windows.h>
#else					// Unix/Linux
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#endif


int recv_size = 0;
int s_size = 0;
struct sockaddr_in server, client;
BSD tmp;


#ifdef WIN32
	SOCKET Socket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;
	WSAData wsaData;
#else
	int Socket = -1;
	int ClientSocket = -1
#endif

bool InitSocket(void)
{
	bool check = false;
	s_size = sizeof(struct sockaddr_in);
#ifdef WIN32
	printf("Initialising Winsock...\n");
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0)
	{
		WSACleanup();
		printf("FAILED \n");
		check = false;
	}
	else
	{
		printf("Initialised Winsock! \n");
		check = true;
	}
#endif

	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	 	// returns a socket descriptor which can be used in other network commands;

	if(INVALID_SOCKET == Socket)
	{
#ifdef WIN32
		printf("Could not create socket : %d" , WSAGetLastError());
		closesocket(Socket);
		WSACleanup();
		check = false;
#else
		printf("FAILED to create socket : %s \n",strerror(errno));
		close(Socket);
		check = false;
#endif
	}
	else
		check = true;
	return check;
}


void SetServer(const char* IP,int port,int num_listeners)
{
	memset(&server, 0, sizeof(server));
	server.sin_addr.s_addr = htonl(INADDR_ANY); //inet_addr(IP); setting with IP;
	server.sin_family = AF_INET;
	server.sin_port = htons( port );

	if(bind(Socket,(struct sockaddr *)&server, sizeof(server)) < 0)
		{
#ifdef WIN32
			printf("Bind FAILED : %d\n", WSAGetLastError());
#else
			printf("Bind FAILED : %s\n",strerror(errno));
#endif
		}
		else
			printf("Bind DONE \n");
	listen(Socket,num_listeners);
	printf("Waiting for incoming connections... \n");
}

DWORD WINAPI SocketHandler(void* lp)        // TODO: Different on UNIX system
{
    int* sock = (int*)lp;

    char buffer[1024];
    int buffer_len = 1024;
    int bytecount;

    memset(buffer,0,buffer_len);
    bytecount = recv(*sock,buffer,buffer_len,0);
    if(bytecount == SOCKET_ERROR)
    {
        printf("Error sending data %d\n",WSAGetLastError());
        goto FINISH;
    }
FINISH:
    free(sock);
    return 0;

}

bool AcceptConn(void)
{
	bool check = false;
	s_size = sizeof(struct sockaddr_in);

    ClientSocket = accept(Socket,(struct sockaddr* )&client,&s_size);

	if( INVALID_SOCKET == ClientSocket)
	{
		printf("Error for connection...\n");
		check = false;
	}
	else
	{
		printf("Server got connection from client %s \n", inet_ntoa(client.sin_addr));
		CreateThread(0,0,&SocketHandler,(void*)ClientSocket,0,0);
		check = true;
	}
	return check;
}

#endif /* HEADERS_SERVERCOMPONENTS_H_ */










