#include "ClientList.h"

typedef vector<PlayerSocket*>::iterator Iter;

ClientList::ClientList(int type)
{
	InitializeCriticalSection(&g_cs);//初始化g_cs的成员 
	mapType = type;
}

ClientList::~ClientList()
{
	DeleteCriticalSection(&g_cs);//删除关键段 
}

void ClientList::add(PlayerSocket* socket)
{
	if (socket != NULL)
	{
		EnterCriticalSection(&g_cs);//进入关键段
		_list.push_back(socket);
		LeaveCriticalSection(&g_cs);//退出关键段  
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
	EnterCriticalSection(&g_cs);//进入关键段
	if (iter != _list.end())
	{
		delete *iter;
		iter=_list.erase(iter);
	}
	LeaveCriticalSection(&g_cs);//退出关键段  
}

Iter ClientList::find(PlayerSocket* socket)
{
	EnterCriticalSection(&g_cs);//进入关键段
	Iter iter = _list.begin();
	while (iter != _list.end())
	{
		if (*iter == socket)
		{
			LeaveCriticalSection(&g_cs);//退出关键段 
			return iter;
		}
		iter++;
	}
	LeaveCriticalSection(&g_cs);//退出关键段  
	return iter;
}

void ClientList::clear()
{
	EnterCriticalSection(&g_cs);//进入关键段
	for (int i = _list.size() - 1; i >= 0; i--)
	{
		delete _list[i];
	}
	_list.clear();
	LeaveCriticalSection(&g_cs);//退出关键段  
}
void ClientList::release(PlayerSocket* socket)
{
	Iter iter = find(socket);
	EnterCriticalSection(&g_cs);//进入关键段
	if (iter != _list.end())
	{
		iter = _list.erase(iter);
	}
	LeaveCriticalSection(&g_cs);//退出关键段  
}