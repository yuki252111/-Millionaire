#ifndef __CLIENT_LIST_H__
#define __CLIENT_LIST_H__

#include "PlayerSocket.h"
#include <vector>
#include <assert.h>
using namespace std;

class ClientList{
public:
	typedef vector<PlayerSocket*>::iterator Iter;

	void add(PlayerSocket* socket);

	int count() const;

	PlayerSocket* operator[](size_t index);

	void remove(PlayerSocket* socket);

	Iter find(PlayerSocket* socket);

	void release(PlayerSocket* socket);

	void clear();

	~ClientList();
	 ClientList(int type);
private:
	CRITICAL_SECTION g_cs;
	vector<PlayerSocket*> _list;
	int mapType;
};

#endif