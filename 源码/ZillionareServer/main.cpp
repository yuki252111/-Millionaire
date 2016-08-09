#include "PlayerSocket.h"
#include "Server.h"
#include <iostream>
#include "ClientList.h"

#include <windows.h>
#include <process.h>
#include <iostream>
#include <mutex>
#pragma comment(lib,"ws2_32.lib") 
using namespace std;

const int port = 7788;
CRITICAL_SECTION m_ss;

void recv(PVOID pt)
{
	PlayerSocket* csocket = (PlayerSocket*)pt;
	if (csocket != NULL)
	{
		while (csocket->mapType == -1||csocket->getEnemy() == NULL || csocket->isStart == false || csocket->getEnemy()->isStart == false)
		{
			char temp[bufferSize];
			memset(temp, 0, bufferSize);
			int count = recv(csocket->getPSocket(), temp, bufferSize, 0);
			if (count <= 0)
			{
				switch (csocket->mapType){
				case 0:
					Server::beachlist->release(csocket);
					break;
				case 1:
					Server::sealist->release(csocket);
					break;
				case 2:
					Server::snowlist->release(csocket);
					break;
				}
				Server::playerCount = Server::playerCount - 1;
				Server::list->remove(csocket);
				cout << "一个用户下线，在线人数：" << Server::playerCount << endl;
				_endthread(); //用户下线，终止接收数据线程
			}
			if (csocket->mapType == -1)
			{
				std::string str(temp);
				size_t found = str.find("SelectedMap");
				if (found != std::string::npos)
				{
					int index = str.find("-");
					csocket->mapType = atoi(str.substr(index + 1, 1).c_str());
					switch (csocket->mapType){
					case 0:
						Server::beachlist->add(csocket);
						break;
					case 1:
						Server::sealist->add(csocket);
						break;
					case 2:
						Server::snowlist->add(csocket);
						break;
					}
				}
			}
			else if (csocket->getEnemy() == NULL)
			{
				std::string str(temp);
				size_t found = str.find("Position");
				if (found != std::string::npos)
				{
					int index = str.find("-");
					csocket->pos = atoi(str.substr(index + 1, str.length() - index - 1).c_str());
				}
			}
			else if (csocket->isStart == false)
			{
				std::string str(temp);
				if (str == "isStart-1")
				{
					csocket->isStart = true;
					char temp[bufferSize];
					memset(temp, 0, bufferSize);
					sprintf_s(temp, "OK");
					send(csocket->getPSocket(), temp, bufferSize, 0);
				}
			}

		}

		int count = csocket->Receive();
		if (count <= 0)
		{
			switch (csocket->mapType){
			case 0:
				Server::beachlist->release(csocket);
				break;
			case 1:
				Server::sealist->release(csocket);
				break;
			case 2:
				Server::snowlist->release(csocket);
				break;
			}
			Server::playerCount = Server::playerCount - 1;
			Server::list->remove(csocket);
			cout << "一个用户下线，在线人数：" << Server::playerCount << endl;
			_endthread(); //用户下线，终止接收数据线程
		}
	}
}
void enterIntoGame(PVOID pt)
{
	while (1)
	{
		if (Server::beachlist->count() >= 2)
		{
			int listSize = Server::beachlist->count();
			PlayerSocket* player1 = NULL;
			PlayerSocket* player2 = NULL;
			vector<PlayerSocket*>players;
			int pos1 = -1;
			int pos2 = -1;
			for (int i = 0; i < listSize-1; i++)
			{
				pos1 = (*Server::beachlist)[i]->pos;
				for (int j = i + 1; j < listSize;j++)
				{
					pos2 = (*Server::beachlist)[j]->pos;
					if (abs(pos1 - pos2) == 1 && min(pos1, pos2) % 2 == 0)
					{
						player1 = (*Server::beachlist)[i];
						player2 = (*Server::beachlist)[j];

						players.push_back(player1);
						players.push_back(player2);

						player1->setEnemy(player2);
						player2->setEnemy(player1);

						char temp1[bufferSize];
						memset(temp1, 0, bufferSize);
						sprintf_s(temp1, "isStart:1:1");

						char temp2[bufferSize];
						memset(temp2, 0, bufferSize);
						sprintf_s(temp2, "isStart:1:0");
						send(player1->getPSocket(), temp1, bufferSize, 0);
						send(player2->getPSocket(), temp2, bufferSize, 0);
					}
				}
			}
			for (int i = 0; i < players.size();i++)
			{
				Server::beachlist->release(players[i]);
			}
			
		}
		if (Server::sealist->count() >= 2)
		{
			int listSize = Server::sealist->count();
			PlayerSocket* player1 = NULL;
			PlayerSocket* player2 = NULL;
			vector<PlayerSocket*>players;
			int pos1 = -1;
			int pos2 = -1;
			for (int i = 0; i < listSize - 1; i++)
			{
				pos1 = (*Server::sealist)[i]->pos;
				for (int j = i + 1; j < listSize; j++)
				{
					pos2 = (*Server::sealist)[j]->pos;
					if (abs(pos1 - pos2) == 1 && min(pos1, pos2) % 2 == 0)
					{
						player1 = (*Server::sealist)[i];
						player2 = (*Server::sealist)[j];

						players.push_back(player1);
						players.push_back(player2);

						player1->setEnemy(player2);
						player2->setEnemy(player1);

						char temp1[bufferSize];
						memset(temp1, 0, bufferSize);
						sprintf_s(temp1, "isStart:1:1");

						char temp2[bufferSize];
						memset(temp2, 0, bufferSize);
						sprintf_s(temp2, "isStart:1:0");
						send(player1->getPSocket(), temp1, bufferSize, 0);
						send(player2->getPSocket(), temp2, bufferSize, 0);
					}
				}
			}
			for (int i = 0; i < players.size(); i++)
			{
				Server::sealist->release(players[i]);
			}
		}
		if (Server::snowlist->count() >= 2)
		{
			int listSize = Server::snowlist->count();
			PlayerSocket* player1 = NULL;
			PlayerSocket* player2 = NULL;
			vector<PlayerSocket*>players;
			int pos1 = -1;
			int pos2 = -1;
			for (int i = 0; i < listSize - 1; i++)
			{
				pos1 = (*Server::snowlist)[i]->pos;
				for (int j = i + 1; j < listSize; j++)
				{
					pos2 = (*Server::snowlist)[j]->pos;
					if (abs(pos1 - pos2) == 1 && min(pos1, pos2) % 2 == 0)
					{
						player1 = (*Server::snowlist)[i];
						player2 = (*Server::snowlist)[j];

						players.push_back(player1);
						players.push_back(player2);

						player1->setEnemy(player2);
						player2->setEnemy(player1);

						char temp1[bufferSize];
						memset(temp1, 0, bufferSize);
						sprintf_s(temp1, "isStart:1:1");

						char temp2[bufferSize];
						memset(temp2, 0, bufferSize);
						sprintf_s(temp2, "isStart:1:0");
						send(player1->getPSocket(), temp1, bufferSize, 0);
						send(player2->getPSocket(), temp2, bufferSize, 0);
					}
				}
			}
			for (int i = 0; i < players.size(); i++)
			{
				Server::snowlist->release(players[i]);
			}
		}
		Sleep(500);
	}
}
void refreshGameHall(PVOID pt)
{
	while (1)
	{
		
		std::string str1 = "Poses";
		for (int i = 0; i < Server::beachlist->count(); i++)
		{
			str1 += ":";
			char temp[bufferSize];
			memset(temp, 0, bufferSize);
			sprintf_s(temp, "%d",(*Server::beachlist)[i]->pos);
			str1 += std::string(temp);
			
		}
		char temp[bufferSize];
		memset(temp, 0, bufferSize);
		sprintf_s(temp, "%s", str1.c_str());
		for (int i = 0; i < Server::beachlist->count(); i++)
		{
			send((*Server::beachlist)[i]->getPSocket(), temp, bufferSize, 0);
		}
		
		std::string str2 = "Poses";
		for (int i = 0; i < Server::sealist->count(); i++)
		{
			str2 += ":";
			char temp[bufferSize];
			memset(temp, 0, bufferSize);
			sprintf_s(temp, "%d", (*Server::sealist)[i]->pos);
			str2 += std::string(temp);
			
		}
		memset(temp, 0, bufferSize);
		sprintf_s(temp, "%s", str2.c_str());
		for (int i = 0; i < Server::sealist->count(); i++)
		{
			send((*Server::sealist)[i]->getPSocket(), temp, bufferSize, 0);
		}
		
		std::string str3 = "Poses";
		for (int i = 0; i < Server::snowlist->count(); i++)
		{
			str3 += ":";
			char temp[bufferSize];
			memset(temp, 0, bufferSize);
			sprintf_s(temp, "%d", (*Server::snowlist)[i]->pos);
			str3 += std::string(temp);

		}
		memset(temp, 0, bufferSize);
		sprintf_s(temp, "%s", str3.c_str());
		for (int i = 0; i < Server::snowlist->count(); i++)
		{
			send((*Server::snowlist)[i]->getPSocket(), temp, bufferSize, 0);
		}
		Sleep(500);
	}
}
int main()
{
	Server server;
	InitializeCriticalSection(&m_ss);

	bool isStart = server.Start(port);
	if (isStart)
	{
		cout << "server start success..." << endl;
	}
	else
	{
		cout << "server start error" << endl;
	}
	_beginthread(enterIntoGame,0,&server);
	_beginthread(refreshGameHall, 0, &server);
	while (1)
	{
		PlayerSocket* csocket = server.Accept();

		cout << "新上线一个用户，在线人数：" << Server::playerCount << endl;
		_beginthread(recv, 0, csocket);//启动一个接收数据的线程
	}

	getchar();
	return 0;
}