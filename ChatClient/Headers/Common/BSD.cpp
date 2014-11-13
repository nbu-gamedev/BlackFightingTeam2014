#include "BSD.h"
const int MAX_SIZE = 2000;

BSD::BSD(void)
{
	recv_size = 0;
	s_size = 0;
#ifdef WIN32
	Socket = INVALID_SOCKET;
	ClientSocket = INVALID_SOCKET;
#else
	Socket = -1;
	ClientSocket = -1
#endif
}

// Creating the Socket

bool BSD::InitSocket(void)
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

// Setting the ip and port and bind it

void BSD::SetServer(const char* IP,int port,int num_listeners)
{
	memset(&server, 0, sizeof(server));
	server.sin_addr.s_addr = inet_addr(IP);
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
}

// Bind IP address and port for all connections...

void BSD::Bind(void)
{
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
}

// Waiting for connections...

bool BSD::AcceptConn(void)
{
	bool check = false;

	if(0 > (ClientSocket = accept(Socket,(struct sockaddr *)&client,&s_size)))
	{
		printf("Error for connection...\n");
		check = false;
	}
	else
	{
		printf("Server got connection from client %s \n", inet_ntoa(client.sin_addr));
		check = true;
	}
	return check;
}

// Receive strings with max size  2000 chars;

bool BSD::RecvStrServer(const char* &str)
{
	char recv_data[MAX_SIZE];
	bool check = false;

	if( 0 > (recv_size = recv(ClientSocket, recv_data,2000,0)))
	{
#ifdef WIN32
		printf("Connection reset by peer : %d\n", WSAGetLastError());
		closesocket(ClientSocket);
#else
		printf("Connection reset by peer : %s\n",strerror(errno));
		close(ClientSocket);
#endif
		check = false;
	}
	else if(recv_size == 0)
	{
		printf("Connection closed\n");
		check = false;
	}
	else
	{
		recv_data[recv_size] = '\0';
		str = recv_data;
		printf("Client[%s] : %s\n",inet_ntoa(client.sin_addr),recv_data);
		check = true;
	}
	return check;
}

bool BSD::RecvStrClient(const char* &str)
{
	char recv_data[MAX_SIZE];
	bool check = false;

	if( 0 > (recv_size = recv(Socket, recv_data,2000,0)))
	{
#ifdef WIN32
		printf("Connection reset by peer : %d\n", WSAGetLastError());
		closesocket(ClientSocket);
#else
		printf("Connection reset by peer : %s\n",strerror(errno));
		close(ClientSocket);
#endif
		check = false;
	}
	else if(recv_size == 0)
	{
		printf("Connection closed\n");
		check = false;
	}
	else
	{
		recv_data[recv_size] = '\0';
		str = recv_data;
		printf("Client[%s] : %s\n",inet_ntoa(client.sin_addr),recv_data);
		check = true;
	}
	return check;
}
// Sending string  and checking if its empty;

bool BSD::SendStrClient(const char* str)
{
	bool check = false;

	if((send(Socket,str,strlen(str),0)) < 0)
		{
			printf("Send failed\n");
			check = false;
		}
		else
			check = true;
	return check;
}

bool BSD::SendStrServer(const char* str)
{
	bool check = false;

	if((send(ClientSocket,str,strlen(str),0)) < 0)
		{
			printf("Send failed\n");
			check = false;
		}
		else
			check = true;
	return check;
}

// Connect to server

bool BSD::Connect(const char* IP, int port)
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

// Close connection with the server

void BSD::ShutDownConn(void)
{
	if(0 > (shutdown(Socket,SD_SEND)))
	{
		printf("Shutdown failed with error: %d\n", WSAGetLastError());
#ifdef WIN32
		closesocket(Socket);
		WSACleanup();
#else
		close(Socket);
#endif
	}
	else
		printf("Connection closed. \n");
}

void BSD::Close(void)
{
#ifdef WIN32
		closesocket(Socket);
		WSACleanup();
#else
		close(Socket);
#endif
}












