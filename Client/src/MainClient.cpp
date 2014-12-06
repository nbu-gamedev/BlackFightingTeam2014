//============================================================================
// Name        : MainClient.cpp
// Author      : Ivaylo, Niki, Mitko
// Version     : 0.1
// Copyright   : Never stop dreaming!
// Description : Black Fithing Team {BFT}
//============================================================================


#include <stdio.h>
#include <string>
#include <iostream>
#include "Common/BSD.h"
#include "ClientComponents.h"
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
	BSD Test(Socket);
	InitSocket();
	char send_message[MAX_SIZE];
	if(Connect("127.0.0.1",8888))
	{
		printf("Enter data for the Server: ");
		while(1)
		{
				fgets(send_message, MAX_SIZE-1, stdin);

				if(Test.SendData(send_message,Socket))
				{
					if(!Test.ReciveData(Socket))
						break;
				}
				else
				{
					printf("Failed to send the packet...\n");
					break;
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
