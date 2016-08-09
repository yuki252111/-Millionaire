#include "PlayerSocket.h"
#include <iostream>
#include <stdio.h>

PlayerSocket::PlayerSocket() :pSocket(NULL), enemy(NULL)
{
	buffer1 = new char[bufferSize];
	memset(buffer1, 0, bufferSize);

	buffer2 = new char[bufferSize];
	memset(buffer2, 0, bufferSize);
	
	mapType = -1;
	pos = -1;
	enemy = NULL;
	isStart = false;
}
PlayerSocket::~PlayerSocket()
{
	if (buffer1 != NULL)
	{
		delete buffer1;
		buffer1 = NULL;
	}
	if (buffer2 != NULL)
	{
		delete buffer2;
		buffer2 = NULL;
	}
	if (pSocket != NULL)
	{
		shutdown(pSocket, SocketEnum::Both);
		closesocket(pSocket);
	}
}
void PlayerSocket::setPSocket(SOCKET socket)
{
	this->pSocket = socket;
}
SOCKET PlayerSocket::getPSocket()
{
	return pSocket;
}
int PlayerSocket::Receive()
{
	int count = 0;
	while (true)
	{
		count = recv(pSocket, buffer1, bufferSize,0);
		if (count <= 0)
		{
			memset(buffer2, 0, bufferSize);
			std::string str = "GameOver-0";
			for (int i = 0; i < str.length(); i++)
				*(buffer2 + i) = str[i];
			send(enemy->getPSocket(), buffer2, bufferSize, 0);

			break;
		}
		std::string str(buffer1);
		size_t found1 = str.find("Player");
		size_t found2 = str.find("Player1");
		size_t found3 = str.find("Player2");
		if (found1!=std::string::npos&&found2==std::string::npos&&found3==std::string::npos)
		{
			memset(buffer2, 0, bufferSize);
			std::string str2 = str.replace(found1, 6, "Enemy");
			for (int i = 0; i < str2.length(); i++)
				*(buffer2 + i) = str2[i];
		}
		else
		{ 
			memset(buffer2, 0, bufferSize);
			for (int i = 0; i < str.length(); i++)
				*(buffer2 + i) = str[i];
		}
		send(pSocket, buffer1, bufferSize, 0);
		send(enemy->getPSocket(), buffer2, bufferSize,0);
	}
	return count;
}
int PlayerSocket::Send(char* buffer)
{
	int count = 0;
	count = send(pSocket, buffer, bufferSize, 0);
	return count;
}
void PlayerSocket::setEnemy(PlayerSocket* enemy)
{
	this->enemy = enemy;
}
PlayerSocket* PlayerSocket::getEnemy()
{
	return enemy;
}