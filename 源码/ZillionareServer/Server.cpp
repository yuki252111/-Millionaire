#include "Server.h"

const int maxConns = 150;
ClientList* Server::beachlist = NULL;
ClientList* Server::sealist = NULL;
ClientList* Server::snowlist = NULL;
ClientList* Server::list = NULL;
int Server::playerCount = 0;
std::vector<std::string> Server::msg;

Server::Server()
{
	beachlist = new ClientList(0);
	sealist = new ClientList(1);
	snowlist = new ClientList(2);
	list = new ClientList(0);
	playerCount = 0;
	msg.clear();
}
bool Server::Start(int port)
{
	isStart = true;
	socketError = SocketEnum::Success;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)//��ʼ���׽���DLL
	{
		SetSocketError(SocketEnum::WSAStartupError);
		isStart = false;
	}
	if (isStart)
	{
		if ((ssocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET){
			SetSocketError(SocketEnum::InvalidSocket);
			isStart = false;
		}
	}
	if (isStart)
	{
		//��ʼ��ָ�����ڴ�����
		memset(&serverAddress, 0, sizeof(sockaddr_in));
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		if (port>0)
		{
			serverAddress.sin_port = htons(port);
		}
		else
		{
			SetSocketError(SocketEnum::InvalidPort);
			isStart = false;
		}
	}
	if (isStart)
	{
		//��
		if (bind(ssocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR){
			SetSocketError(SocketEnum::BindError);
		}
		else
		{
			if (listen(ssocket,maxConns ) == SOCKET_ERROR)//��������״̬
			{
				SetSocketError(SocketEnum::ListenError);
			}
		}
	}

	return isStart;
}

void Server::SetSocketError(SocketEnum::SocketError error)
{
	socketError = error;
}

PlayerSocket* Server::Accept()
{
	PlayerSocket* psocket = new PlayerSocket();
	struct sockaddr_in clientAddress;//�����Ϳͻ���ͨ�ŵ��׽��ֵ�ַ
	int addrlen = sizeof(clientAddress);
	memset(&clientAddress, 0, addrlen);//��ʼ����ſͻ�����Ϣ���ڴ� 
	SOCKET socket;
	if ((socket = accept(ssocket, (sockaddr*)&clientAddress, &addrlen)) != INVALID_SOCKET)
	{
		psocket->setPSocket(socket);
		playerCount++;
		list->add(psocket);
	}
	return psocket;
}

Server::~Server()
{
	Close();
	delete beachlist;
	delete sealist;
	delete snowlist;
	delete list;
	beachlist = NULL;
	sealist = NULL;
	snowlist = NULL;
	list = NULL;
	playerCount = 0;
}

bool Server::ShutDown(SocketEnum::ShutdownMode mode)
{
	SocketEnum::SocketError nRetVal = (SocketEnum::SocketError)shutdown(ssocket, SocketEnum::Both);
	return (nRetVal == SocketEnum::Success) ? true : false;
}

void Server::Close()
{
	ShutDown(SocketEnum::Both);
	if (closesocket(ssocket) != SocketEnum::Error)
	{
		ssocket = INVALID_SOCKET;
	}
	WSACleanup();//�����׽���ռ�õ���Դ
}

