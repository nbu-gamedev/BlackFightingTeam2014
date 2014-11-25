//============================================================================
// Name        : MainServer.cpp
// Author      : Ivaylo, Niki, Mitko
// Version     : 0.1
// Copyright   : Never stop dreaming!
// Description : Black Fithing Team {BFT}
//============================================================================

/*
 * Add to linker -ws2_32 so that MINGW can use winsock or winsock2
 */

#include <stdio.h>
#include <string>
#include "Headers/ServerComponents.h"
#include "Headers/Common/BSD.h"
const int MAX_SIZE = 2000;

int main() {
	//const char *message = " "
	InitSocket();
	SetServer("127.0.0.1",8888,3);

	while(1){
		if(AcceptConn())
		{
			BSD tmp(ClientSocket);
			printf("Connections accepted \n");
			while(1)
			{
				if(tmp.ReciveData(ClientSocket))
				{
					if(!tmp.SendData("Hello",ClientSocket))
						break;
				}
				else
					break;
				if(ClientSocket < 0)
				{
					printf("accept failed with error : %d\n", WSAGetLastError());
					break;
				}
			}

		}
	}

#ifdef WIN32
		WSACleanup();
#else
		close(Socket)
#endif

	system("PAUSE");
	return 0;
}
