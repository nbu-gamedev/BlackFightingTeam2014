


#include "BSDv2.h"
#include <iostream>
#include <time.h>


bool ComCon::Init(void)
{
	bool check = false;

#ifdef WIN32
#ifdef NetDebug
	std::cout << "Initialising Winsock...\n";
#endif
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0)
	{
		WSACleanup();
#ifdef NetDebug
		std::cout << "FAILED \n";
#endif
		check = false;
	}
	else
	{
#ifdef NetDebug
		std::cout << "Initialised Winsock! \n";
#endif
		check = true;
	}
#endif // WIN32

	MasterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	 	// returns a socket descriptor which can be used in other network commands;

	if(INVALID_SOCKET == MasterSocket)
	{
#ifdef WIN32
		closesocket(MasterSocket);
		WSACleanup();
		check = false;
#else
#ifdef NetDebug
		std::cout << "FAILED to create socket : " << strerror(errno) << std::endl;
#endif
		close(MasterSocket);
		check = false;
#endif // WIN32
	}
	else
    {
//        DWORD nonBlocking = 1;
//        if ( ioctlsocket( MasterSocket, FIONBIO, &nonBlocking ) != 0 )
//        {
//#ifdef NetDebug
//            std::cout << "failed to set non-blocking\n";
//#endif
//            return false;
//        }

        check = true;
    }

#ifdef BFT_client
	do{
//	check = this->Connect("127.0.0.1",14000);
	check = this->Connect("25.149.221.83",2620);
	}while(false == check);
#elif defined BFT_server
//	this->SetServer("127.0.0.1",14000,100);
	this->SetServer("25.149.221.83",2620,100);
#else
#error "NET: BFT platform not defined"
#endif
	return check;
}

void ComCon::Process(void)
{
#ifdef BFT_client
//client process

#elif defined BFT_server

	if(this->AcceptConn())
	{
		//spam new user data structure
		std::cout << "New connections accepted \n";
	}
//	std::cout << "The last socet error was: " << WSAGetLastError()<< std::endl;

#else
#error "NET: BFT platform not defined"
#endif
}
netData master;
int ComCon::sendData(
#ifdef BFT_server
std::list<netData*>& activeUsers,
#endif
message_t& mess)
{
#ifdef BFT_server
	std::list<netData*>::iterator it = activeUsers.begin();
    std::list<netData*>::iterator ActUserEnd = activeUsers.end();
#endif
	int res;
//	int bytesSend;
//    unsigned int currentTime = 1420807713; //1420807713 = 09.01.2015 @ 12:48


#ifdef BFT_server
    for(;it!=ActUserEnd; it++)
	{
		res = this->sendData(*(*it), mess);
//        bytesSend = send((*it)->m_socket,SendMessage,8+mess.m_dataSize,0);

#elif defined BFT_client
	{

		master.m_socket = MasterSocket;
		res = sendData(master, mess);
//		bytesSend = send(MasterSocket,SendMessage,8+mess.m_dataSize,0);
#endif

	}
#ifdef NetDebug
//	std::cout <<"send -> mess ID: " << mess.mID <<" __ mess size: " << mess.m_dataSize << std::endl;
#endif
	return res;
}

int ComCon::sendData(netData& user, message_t& mess)
{
	int res = 0;
	int bytesSend;
	int netError;
	char SendMessage[cMaxMessInputSize];


	if(cMaxMessInputSize > 8+mess.m_dataSize)
	{
		prepairMessage(SendMessage, mess);
	}
	else
	{
		std::cout<<"Message is too big to be send\n";
	}

	bytesSend = send(user.m_socket,SendMessage,8+mess.m_dataSize,0);
	netError = WSAGetLastError();

	if((0!=netError)&&(WSAEWOULDBLOCK !=netError))
	{	//error in the socket except
		throw netError;
	}
	if(bytesSend != 8+mess.m_dataSize)
	{
		std::cout<<"Error sending\n";
		res = 1;
	}

	return res;
}

void ComCon::prepairMessage(char* sendStr, message_t& mess)
{
	time_t currentTime = time(NULL);

	sendStr[0] = char(currentTime>>24);
    sendStr[1] = char(currentTime>>16);
    sendStr[2] = char(currentTime>>8);
    sendStr[3] = char(currentTime);

    sendStr[4] = char(mess.mID>>8);
    sendStr[5] = char(mess.mID);

    sendStr[6] = char(mess.m_dataSize>>8);
    sendStr[7] = char(mess.m_dataSize);

    if(0 < mess.m_dataSize)
    {
		memcpy(&(sendStr[8]), mess.data, mess.m_dataSize);
    }
}


int ComCon::receiveData(
#ifdef BFT_server
		netData& user,
#endif
		message_t& mess)
{
    int recv_size;
	int netError;
    char recv_data[cMaxMessInputSize];

#ifdef BFT_server
    recv_size = recv(user.m_socket, recv_data,8,0);
#elif defined BFT_client
	recv_size = recv(MasterSocket, recv_data,8,0);
#endif

	netError = WSAGetLastError();
	if((0!=netError)&&(WSAEWOULDBLOCK !=netError))
	{	//error in the socket except
		throw netError;
	}

    if( 8 == recv_size)
    {//data is present

		mess.rewindCursor();

        mess.mTime = int(recv_data[0])<<24 | int(recv_data[1])<<16 | int(recv_data[2])<<8 | int(recv_data[3]);
        mess.mID = int(recv_data[4])<<8 | int(recv_data[5]);
        mess.m_dataSize = int(recv_data[6])<<8 | int(recv_data[7]);

        if(0 < mess.m_dataSize)
        {
#ifdef BFT_server

            recv_size = recv(user.m_socket, recv_data,mess.m_dataSize,0);
#elif defined BFT_client
			recv_size = recv(MasterSocket, recv_data,mess.m_dataSize,0);
#endif
			netError = WSAGetLastError();

			if((0!=netError)&&(WSAEWOULDBLOCK !=netError))
			{	//error in the socket except
				throw netError;
			}

            if(recv_size == mess.m_dataSize)
            {
                memcpy(mess.data, &recv_data, recv_size);
            }
            else
            {
#ifdef NetDebug
                std::cout<<"Broken package received"<<std::endl;
#endif
                return 2;//error ocured
            }
        }
//		std::cout <<"Receive -> mess ID: " << mess.mID <<" __ mess size: " << mess.m_dataSize << std::endl;
    }
    else if(0 < recv_size)
    {
#ifdef NetDebug
        std::cout<<"Broken package received"<<std::endl;
#endif
        return 2;//error ocured
    }
	else
	{
		return 1;// no message received
	}

	return 0;
}

bool ComCon::Connect(const char* IP, int port)
{
	bool check = false;
    struct sockaddr_in server;

	memset(&server, 0, sizeof(server));
	server.sin_addr.s_addr = inet_addr(IP);
	server.sin_family = AF_INET;
	server.sin_port = htons( port );

	if(connect(MasterSocket, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
#ifdef NetDebug
		std::cout<<"Connect Error\n";
#endif
#ifdef WIN32
		closesocket(MasterSocket);
		WSACleanup();
#else
		close(MasterSocket);
#endif // WIN32
		check = false;
	}
	else
	{
	    DWORD nonBlocking = 1;
        if ( ioctlsocket( MasterSocket, FIONBIO, &nonBlocking ) != 0 )
        {
#ifdef NetDebug
            std::cout << "failed to set non-blocking\n";
#endif
            return false;
        }
#ifdef NetDebug
		std::cout << "Connected\n";
#endif
		check = true;
	}
	return check;
}

#ifdef BFT_server
void ComCon::SetServer(const char* IP,int port,int num_listeners)
{
    struct sockaddr_in server;

	memset(&server, 0, sizeof(server));
	server.sin_addr.s_addr = htonl(INADDR_ANY); //inet_addr(IP); setting with IP;
	server.sin_family = AF_INET;
	server.sin_port = htons( port );

	if(bind(MasterSocket,(struct sockaddr *)&server, sizeof(server)) < 0)
		{
#ifdef NetDebug
#ifdef WIN32
			std::cout << "Bind FAILED : " << WSAGetLastError() << std::endl;
#else
			std::cout << "Bind FAILED : " << strerror(errno) << std::endl;
#endif // WIN32
#endif // NetDebug
		}
		else
        {
#ifdef NetDebug
			std::cout << "Bind DONE \n";
#endif
        }

	listen(MasterSocket,num_listeners);
#ifdef NetDebug
	std::cout << "Waiting for incoming connections... \n";
#endif
	DWORD nonBlocking = 1;
    if ( ioctlsocket( MasterSocket, FIONBIO, &nonBlocking ) != 0 )
    {
#ifdef NetDebug
		std::cout << "failed to set non-blocking\n";
#endif
	}
}

bool ComCon::AcceptConn(void)
{
    struct sockaddr_in client;
	bool check = false;
    int s_size = 0;

	SOCKET ClientSocket = INVALID_SOCKET;
	s_size = sizeof(struct sockaddr_in);

    ClientSocket = accept(MasterSocket,(struct sockaddr* )&client,&s_size);

	if( INVALID_SOCKET == ClientSocket)
	{
//		printf("Error for connection...\n");
		check = false;
	}
	else
	{

        DWORD nonBlocking = 1;
        if ( ioctlsocket( ClientSocket, FIONBIO, &nonBlocking ) != 0 )
        {
#ifdef NetDebug
			std::cout << "failed to set non-blocking\n";
#endif
            return false;
        }

		if(NULL != mf_newUserCbk)
		{
			netData newUser;
			newUser.m_socket = ClientSocket;
			newUser.m_addr = client;
			mf_newUserCbk(newUser);//Call the new user creator
			std::cout << "connected: \n";
		}

//TODO: Spam new user stucture
/*        int i=0;
        for(;(i<100)&&(aUsers[i].m_socket != INVALID_SOCKET);i++);
        aUsers[i].m_socket = ClientSocket;
        aUsers[i].m_addr = client;

        activeUsers.push_back(&(aUsers[i]));
*/
#ifdef NetDebug
		std::cout << "Server got connection from client" << inet_ntoa(client.sin_addr) << std::endl;
#endif
		check = true;
	}
	return check;
}


void ComCon::setNewUserCbk(newUserCbk_t fCbk)
{
	mf_newUserCbk = fCbk;
}


#endif //BFT_server

