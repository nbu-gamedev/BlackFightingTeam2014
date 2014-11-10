//============================================================================
// Name        : ChatServer.cpp
// Author      : NewKnight
// Version     :
// Copyright   : Never stop dreaming!
// Description : Hello World in C++, Ansi-style
//============================================================================

/*
 * Add to linker -ws2_32 so that MINGW can use winsock or winsock2
 */

#include <stdio.h>
#include <string>
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

const int MAX_SIZE = 2000;





int main() {
	struct sockaddr_in server, client;
	int c;
	int recv_size;
	char recv_data[2000];
	//char send_message[MAX_SIZE];
	const char *message = " ";

#ifdef WIN32
	SOCKET Socket = INVALID_SOCKET;
	SOCKET NewSocket = INVALID_SOCKET;
	WSAData wsaData;

	printf("Initialising Winsock...\n");
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0)
	{
		WSACleanup();
		printf("FAILED \n");
	}
	else
		printf("Initialised Winsock! \n");
#else
	int Socket = -1;
	int NewSocket = -1;
#endif
	/* Create Socket*/
	Socket = socket(AF_INET, SOCK_STREAM, 0);	 	// returns a socket descriptor which can be used in other network commands;

	if(Socket < 0)
	{
#ifdef WIN32
		printf("Could not create socket : %d" , WSAGetLastError());
		closesocket(Socket);
		WSACleanup();
#else
		printf("FAILED to create socket : %s \n",strerror(errno));
		close(Socket);
#endif
	}

	memset(&server, 0, sizeof(server));
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );

	// Bind
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

	//Listen for incoming connections
	listen(Socket, 3);
	printf("Whaiting for incoming connections ... \n");


	while(1){
		c = sizeof(struct sockaddr_in);
		if((NewSocket = accept(Socket, (struct sockaddr *)&client,&c)) < 0)
		{
			printf("Client failed accepted\n");
			break;
		}
		else{
			printf("Connections accepted \n");
			printf("Server got connection from client %s \n", inet_ntoa(client.sin_addr));
		}

		//printf("Enter data for the Client: ");
		//fgets(send_message, MAX_SIZE-1, stdin);
		while(1)
			{
				if((recv_size = recv(NewSocket, recv_data, 2000,0))<0)
					{
#ifdef WIN32
		printf("Connection reset by peer : %d\n", WSAGetLastError());
#else
		printf("Connection reset by peer : %s\n",strerror(errno));
#endif
						break;
					}
				else if(recv_size == 0)
				{
					printf("Connection closed\n");
				}
				else
				{
					recv_data[recv_size] = '\0';
					printf("Client[%s] : %s\n",inet_ntoa(client.sin_addr),recv_data);
				}

				send(NewSocket, message, strlen(message), 0);
			}
		if(NewSocket < 0)
		{
			printf("accept failed with error : %d\n", WSAGetLastError());
			break;
		}
		closesocket(NewSocket);
	}

#ifdef WIN32
		closesocket(Socket);
		WSACleanup();
#else
		close(Socket)
#endif

	system("PAUSE");
	return 0;
}
