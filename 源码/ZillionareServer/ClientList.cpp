#include "ClientList.h"

typedef vector<PlayerSocket*>::iterator Iter;

ClientList::ClientList(int type)
{
	InitializeCriticalSection(&g_cs);//��ʼ��g_cs�ĳ�Ա 
	mapType = type;
}

ClientList::~ClientList()
{
	DeleteCriticalSection(&g_cs);//ɾ���ؼ��� 
}

void ClientList::add(PlayerSocket* socket)
{
	if (socket != NULL)
	{
		EnterCriticalSection(&g_cs);//����ؼ���
		_list.push_back(socket);
		LeaveCriticalSection(&g_cs);//�˳��ؼ���  
	}
}

int ClientList::count() const
{
	return _list.size();
}

PlayerSocket* ClientList::operator[](size_t index)
{
	if (index >= 0 && index < _list.size())
		return _list[index];
	else
		return NULL;
}

void ClientList::remove(PlayerSocket* socket)
{
	Iter iter = find(socket);
	EnterCriticalSection(&g_cs);//����ؼ���
	if (iter != _list.end())
	{
		delete *iter;
		iter=_list.erase(iter);
	}
	LeaveCriticalSection(&g_cs);//�˳��ؼ���  
}

Iter ClientList::find(PlayerSocket* socket)
{
	EnterCriticalSection(&g_cs);//����ؼ���
	Iter iter = _list.begin();
	while (iter != _list.end())
	{
		if (*iter == socket)
		{
			LeaveCriticalSection(&g_cs);//�˳��ؼ��� 
			return iter;
		}
		iter++;
	}
	LeaveCriticalSection(&g_cs);//�˳��ؼ���  
	return iter;
}

void ClientList::clear()
{
	EnterCriticalSection(&g_cs);//����ؼ���
	for (int i = _list.size() - 1; i >= 0; i--)
	{
		delete _list[i];
	}
	_list.clear();
	LeaveCriticalSection(&g_cs);//�˳��ؼ���  
}
void ClientList::release(PlayerSocket* socket)
{
	Iter iter = find(socket);
	EnterCriticalSection(&g_cs);//����ؼ���
	if (iter != _list.end())
	{
		iter = _list.erase(iter);
	}
	LeaveCriticalSection(&g_cs);//�˳��ؼ���  
}