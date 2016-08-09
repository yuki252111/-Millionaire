#ifndef __PLAYER_SOCKET_H__
#define __PLAYER_SOCKET_H__

#include <Windows.h>
#include "SocketEnum.h"

const int bufferSize = 512;

class PlayerSocket{
public :
	PlayerSocket();
	~PlayerSocket();
	int Send(char* buffer);
	int Receive();
	void setPSocket(SOCKET);
	SOCKET getPSocket();
	void setEnemy(PlayerSocket*);
	PlayerSocket* getEnemy();

	int mapType;
	int pos;
	bool isStart;
private:
	SOCKET pSocket;
	char* buffer1;
	char* buffer2;
	PlayerSocket* enemy;
};

#endif