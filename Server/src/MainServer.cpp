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

int main(int argc , char *argv[])
{

    char *message = "ECHO Daemon v1.0 \r\n";
    InitServer();
    addrlen = sizeof(struct sockaddr_in);

    while(TRUE)
    {
        Accept_Connections();
        Clients_Events();
    }
    Close_Server();
    return 0;
}
