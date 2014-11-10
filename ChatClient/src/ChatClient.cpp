//============================================================================
// Name        : ChatClient.cpp
// Author      : NewKnight
// Version     :
// Copyright   : Never stop dreaming!
// Description : Hello World in C++, Ansi-style
//============================================================================


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

const int MAX_SIZE = 1024;

int main(int argc, char **argv) {
	struct sockaddr_in server;
	char recv_data[2000];
	char send_message[MAX_SIZE];
	int recv_size;

#ifdef WIN32
	SOCKET Socket = INVALID_SOCKET;
	WSAData wsaData;
	printf("Initialising Winsick...\n");
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0)
	{
		WSACleanup();
		printf("FAILED \n");
	}
	else
		printf("Initialised Winsock! \n");
#else
	int Socket = -1;
#endif

	Socket = socket(AF_INET, SOCK_STREAM, 0);

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
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );

	if(connect(Socket, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		printf("Connect Error\n");
	}
	else
		printf("Connected\n");

	while(1)
	{
		printf("Enter data for the Server: ");
		fgets(send_message, MAX_SIZE-1, stdin);

		if(send(Socket, send_message, strlen(send_message), 0) < 0)
			{
				printf("Send failed\n");
				break;
			}
			else
			{
				if((recv_size = recv(Socket, recv_data, 2000,0)) < 0)
				{
					printf("Resive failed\n");
					break;
				}
				else if (recv_size > 0)
				{
					recv_data[recv_size] = '\0';
					printf("%s\n",recv_data);
				}
			}
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
