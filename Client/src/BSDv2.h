/*
 *
 *
 *
 *
 */

#ifndef ___BSDv2h___
#define ___BSDv2h___

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

#ifdef BFT_server
#include "boost\function.hpp"
#endif

#include <list>
#include <queue>
#include <string>

#define cMaxMessageLen 100
#define cMaxMessInputSize 2000

typedef enum
{
    eMessID_text = 1,
    eMessID_uName,
    eMessID_pass,
    eMessID_autentOk,
    eMessID_autentNok,
    eMessID_charList,
    eMessID_choosChar,
    eMessID_charStats,
    eMessID_charItemList,
    eMessID_charMoney,
    eMessID_charRanksAndXp,
    eMessID_shoot,
    eMessID_hitKey,
    eMessID_move,
    eMessID_openGame,
	eMessID_done,
	eMessID_crateChar,
	eMessID_startGame,
	eMessID_endGame

}messageID_t;

class message_t
{
public:
    unsigned int mTime;
    unsigned short mID;
    unsigned short m_dataSize;
    unsigned char data[cMaxMessageLen];
	unsigned char* m_readCursor;
	
	message_t(void)
	{
		mID = 0;
		m_dataSize = 0;
		m_readCursor = data;
	}

	inline void rewindCursor(void)
	{
		m_dataSize = 0;
		m_readCursor = data;
	}
	
	inline unsigned int getVarU32 (void)
	{
		unsigned int res = 0;
		if((m_readCursor + 3) < &(data[m_dataSize]))
		{
			res =  static_cast<unsigned int>(*(m_readCursor++)) << 24;
			res |=  static_cast<unsigned int>(*(m_readCursor++)) << 16;
			res |=  static_cast<unsigned int>(*(m_readCursor++)) << 8;
			res |=  static_cast<unsigned int>(*(m_readCursor++));
		}
		
		return res;
	}
	inline unsigned int getVarU16 (void)
	{
		unsigned int res = 0;
		if((m_readCursor + 1) < &(data[m_dataSize]))
		{
			res |=  static_cast<unsigned int>(*(m_readCursor++)) << 8;
			res |=  static_cast<unsigned int>(*(m_readCursor++));
		}
		return res;
		
	}
	inline unsigned int getVarU8 (void)
	{
		unsigned int res = 0;
		if((m_readCursor) < &(data[m_dataSize]))
		{
			res = static_cast<unsigned int>(*(m_readCursor++));
		}
		return res;
	}
	
	inline std::string addString(void)
	{
		std::string recStr = (char*)m_readCursor;
		
		if((m_readCursor + recStr.length() +1) < &(data[m_dataSize]))
		{
			m_readCursor += recStr.length()+1;
			return recStr;
		}
		else
		{
			recStr = "";
			return recStr;
		}
	}


	inline void addVarU32(unsigned int var)
	{
		if(cMaxMessageLen > m_dataSize + 4)
		{
			data[m_dataSize++] = static_cast<unsigned char>(var>>24);
			data[m_dataSize++] = static_cast<unsigned char>(var>>16);
			data[m_dataSize++] = static_cast<unsigned char>(var>>8);
			data[m_dataSize++] = static_cast<unsigned char>(var);
		}

	}

	inline void addVarU16(unsigned short var)
	{
		if(cMaxMessageLen > m_dataSize + 2)
		{
			data[m_dataSize++] = static_cast<unsigned char>(var>>8);
			data[m_dataSize++] = static_cast<unsigned char>(var);
		}
	}

	inline void addVarU8(unsigned char var)
	{
		if(cMaxMessageLen > m_dataSize + 1)
		{
			data[m_dataSize++] = var;
		}
	}

	inline void addString(std::string str)
	{
		unsigned int copyLen = str.length() + 1;
		if(cMaxMessageLen > m_dataSize + copyLen)
		{
			memcpy(&(data[m_dataSize]), str.c_str(), copyLen);
			m_dataSize += copyLen;
		}
	}


};


void addVarU32(unsigned int var);
void addVarU16(unsigned short var);
void addVarU8(unsigned char var);
void addString(std::string str);



class netData
{
public:
    struct sockaddr_in m_addr;
    SOCKET m_socket;

    inline netData()
    {
        memset(&m_addr, 0, sizeof(sockaddr_in));
        m_socket = INVALID_SOCKET;
    }

    inline ~netData()
    {
//        closesocket(m_socket);
    }
};


class ComCon{
private:
    SOCKET MasterSocket;
    WSAData wsaData;

#ifdef BFT_server
	typedef boost::function<void(netData)> newUserCbk_t;
	newUserCbk_t mf_newUserCbk; // call back function for the newly connected user.
#endif

    static ComCon* m_pInstance; //Static instance of the class. The only instance.

    inline ComCon(){
        MasterSocket = INVALID_SOCKET;
#ifdef BFT_server
		mf_newUserCbk = NULL;
#endif
    };  // Private so that it can  not be called



	bool Connect(const char* IP, int port);// connect client to server
#ifdef BFT_server
    void SetServer(const char* IP,int port,int num_listeners);//set the server socket
	bool AcceptConn(void);// Server accepts client connections
#endif
	void sendToAll(void);
	void prepairMessage(char* sendStr, message_t& mess);

public:
    inline ~ComCon(){
        #ifdef WIN32
            WSACleanup();
        #endif // WIN32
    };
    inline static ComCon* Instance(void)
    {
        if (NULL == m_pInstance)   // Only allow one instance of class to be generated.
            m_pInstance = new ComCon;

        return m_pInstance;
    }
#ifdef BFT_server
	void setNewUserCbk(newUserCbk_t fCbk);//register call back function for new user creation.
	//this function shuold be called like this
	//ComCon::Instance()->setNewUserCbk(std::bind(&class::someMethod, this));
#endif
    bool Init(void);// init the sockets and connect to the network
    void Process(void); //doing all the work, accept connections, error handling.

    //user functions
    //set message to be send
    int sendData(
#ifdef BFT_server
		std::list<netData*>& activeUsers,
#endif
		message_t& mess);

	int sendData(netData& user,message_t& mess);

    int receiveData(
#ifdef BFT_server
		netData& user,
#endif
		message_t& mess);

};

#endif //___BSDv2h___
