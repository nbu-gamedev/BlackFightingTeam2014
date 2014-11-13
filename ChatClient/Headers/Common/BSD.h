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
private:
	int recv_size;
	int s_size;
#ifdef WIN32
	SOCKET Socket;
	SOCKET ClientSocket;
	WSAData wsaData;
#else
	int Socket;
	int ClientSocket;
#endif

public:
	BSD();
	//~BSD();
	bool InitSocket();
	void SetServer(const char* IP, int port, int num_listeners);
	void Bind();
	void Close();
	bool AcceptConn();
	bool Connect(const char* IP, int port);
	void ShutDownConn();


	bool SendStrClient(const char* str);
	bool SendStrServer(const char* str);
	bool RecvStrServer(const char* &str);
	bool RecvStrClient(const char* &str);

	struct sockaddr_in server, client;

};
