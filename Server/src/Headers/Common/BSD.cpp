#include "BSD.h"
const int MAX_SIZE = 2000;

BSD::BSD(void)
{
	Socket = -1;
	recv_size = 0;
	s_size = 0;
}

BSD::BSD(SOCKET socket)
{
	Socket = socket;
	recv_size = 0;
	s_size = 0;
}

/*BSD::~BSD()
{
#ifdef WIN32
		printf("Socket Closed. \n");
		closesocket(Socket);
#else
		printf("Socket Closed. \n");
		close(Socket);
#endif
}*/

bool BSD::ReciveData(unsigned int socket)
{
	char recv_data[MAX_SIZE];
	bool check = false;
	int res = recv_size = recv(socket, recv_data,2000,0);

	if( 0 > res)
	{
#ifdef WIN32
		printf("Connection reset by peer : %d\n", WSAGetLastError());
		closesocket(Socket);
#else
		printf("Connection reset by peer : %s\n",strerror(errno));
		close(Socket);
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
		char* str = recv_data;
		printf("Recived Data : %s\n",str);
		check = true;
	}
	return check;
}


bool BSD::SendData(const char* str,unsigned int socket)
{
	bool check = false;
	if((send(socket,str,strlen(str),0)) < 0)
		{
			printf("Send failed\n");
			check = false;
		}
		else
			check = true;
	return check;
}
