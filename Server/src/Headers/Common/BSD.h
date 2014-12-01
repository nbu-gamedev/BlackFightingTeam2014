#ifndef Included_BSD_H
#define Included_BSD_H


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

// Link with ws2_32.lib

class BSD
{
public:
	int recv_size;
	int s_size;
#ifdef WIN32
	SOCKET Socket;
#else
	int Socket;
#endif
	BSD();
	BSD(SOCKET socket);
	//~BSD();

	bool ReciveData(unsigned int socket);
	bool SendData(const char* str,unsigned int socket);
};


#endif // Included_BSD_H
