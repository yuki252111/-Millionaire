#ifndef __SERVER_H__
#define __SERVER_H__

#include "SocketEnum.h"
#include <windows.h>
#include "PlayerSocket.h"
#include "ClientList.h"

class Server
{
public:
	//启动服务器
	bool Start(int port);
	//接收客户端请求
	PlayerSocket* Accept();
	void SetSocketError(SocketEnum::SocketError error);
	Server();
	~Server();
	void Close();
	bool ShutDown(SocketEnum::ShutdownMode mode);

	static ClientList* beachlist;
	static ClientList* sealist;
	static ClientList* snowlist;
	static ClientList* list;
	static int playerCount;
	static std::vector<std::string>msg;
private:
	SOCKET ssocket;
	char* buffer;
	struct sockaddr_in serverAddress;
	SocketEnum::SocketError socketError;
	bool isStart;
	WSADATA wsa;

	
};

#endif