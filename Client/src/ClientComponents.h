/*
 * ClientComponents.h
 *
 *  Created on: 22.11.2014 ã.
 *      Author: NewKnight
 */

#ifndef HEADERS_CLIENTCOMPONENTS_H_
#define HEADERS_CLIENTCOMPONENTS_H_

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

#ifdef WIN32
	SOCKET Socket = INVALID_SOCKET;
	WSAData wsaData;
#else
	int Socket = -1;
#endif

struct sockaddr_in server;

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

	if(Socket < 0)
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
bool Connect(const char* IP, int port)
{
	bool check = false;

	memset(&server, 0, sizeof(server));
	server.sin_addr.s_addr = inet_addr(IP);
	server.sin_family = AF_INET;
	server.sin_port = htons( port );

	if(connect(Socket, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		printf("Connect Error\n");
#ifdef WIN32
		closesocket(Socket);
		WSACleanup();
#else
		close(Socket);
#endif
		check = false;
	}
	else
	{
		printf("Connected\n");
		check = true;
	}
	return check;
}





#endif /* HEADERS_CLIENTCOMPONENTS_H_ */
