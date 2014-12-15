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

const int MAXRECV = 2000;
int recv_size = 0;
int s_size = 0;
struct sockaddr_in server, address;
int max_clients = 30 , listen_num = 3 , activity, addrlen, i, valread;
//set of socket descriptors
fd_set readfds;
//1 extra for null character, string termination
char *buffer;

#ifdef WIN32
	WSADATA wsa_data;
    SOCKET master , new_socket , client_socket[30] , s;
#else
	int Socket = -1;
	int ClientSocket = -1
#endif

bool InitServer(void)
{
     buffer =  (char*) malloc((MAXRECV + 1) * sizeof(char));
     for(i = 0 ; i < max_clients;i++)
        {
            client_socket[i] = 0;
        }
     printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa_data) != 0)
        {
            printf("Failed. Error Code : %d",WSAGetLastError());
            return false;
        }else{
            printf("Initialised.\n");
        }
    //Create a socket
    if((master = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
        {
            printf("Could not create socket : %d" , WSAGetLastError());
            return false;
        }
        else{
            printf("Socket created.\n");
        }
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
    printf("IP: %s\nPORT: %d\n",server.sin_addr.s_addr,server.sin_port);
    //Bing
    if( bind(master ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
        {
            printf("Bind failed with error code : %d" , WSAGetLastError());
            return false;
        }
        else{
            printf("Bind done.\n");
        }
    //Listen to incoming connections
    listen(master , listen_num);
    //Accept and incoming connection
    printf("Waiting for incoming connections...\n");
}

void Accept_Connections(void){
        //clear the socket fd set
        FD_ZERO(&readfds);

        //add master socket to fd set
        FD_SET(master, &readfds);

        //add child sockets to fd set
        for (  i = 0 ; i < max_clients ; i++)
        {
            s = client_socket[i];
            if(s > 0)
            {
                FD_SET( s , &readfds);
            }
        }

        //wait for an activity on any of the sockets, timeout is NULL , so wait indefinitely
        activity = select( 0 , &readfds , NULL , NULL , NULL);
        if ( activity == SOCKET_ERROR )
        {
            printf("select call failed with error code : %d" , WSAGetLastError());
            return;
        }

        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(master , &readfds))
        {
            if ((new_socket = accept(master , (struct sockaddr *)&address, (int *)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
            //add new socket to array of sockets
            for (i = 0; i < max_clients; i++)
            {
                if (client_socket[i] == 0)
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets at index %d \n" , i);
                    break;
                }
            }
        }
}

void Clients_Events(void){
     for (i = 0; i < max_clients; i++)
        {
            s = client_socket[i];
            //if client presend in read sockets
            if (FD_ISSET( s , &readfds))
            {
                //get details of the client
                getpeername(s , (struct sockaddr*)&address , (int*)&addrlen);

                //Check if it was for closing , and also read the incoming message
                //recv does not place a null terminator at the end of the string (whilst printf %s assumes there is one).
                valread = recv( s , buffer, MAXRECV, 0);

                if( valread == SOCKET_ERROR)
                {
                    int error_code = WSAGetLastError();
                    if(error_code == WSAECONNRESET)
                    {
                        //Somebody disconnected , get his details and print
                        printf("Host disconnected unexpectedly , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                        //Close the socket and mark as 0 in list for reuse
                        closesocket( s );
                        client_socket[i] = 0;
                    }
                    else
                    {
                        printf("recv failed with error code : %d" , error_code);
                    }
                }
                if ( valread == 0)
                {
                    //Somebody disconnected , get his details and print
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                    //Close the socket and mark as 0 in list for reuse
                    closesocket( s );
                    client_socket[i] = 0;
                }

                //Echo back the message that came in
                else
                {
                    //add null character, if you want to use with printf/puts or other string handling functions
                    buffer[valread] = '\0';
                    printf("%s:%d - %s \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port), buffer);
                    send( s , buffer , valread , 0 );
                }
            }
        }
}
void Close_Server(void)
{
    closesocket(master);
    closesocket(s);
    WSACleanup();
}

#endif /* HEADERS_SERVERCOMPONENTS_H_ */










