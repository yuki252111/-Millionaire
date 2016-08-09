#ifndef __SOCK_DATA_MANAGER_H__
#define __SOCK_DATA_MANAGER_H__

#include "cocos2d.h"
USING_NS_CC;
//�����࣬���ݴ���
class SocketDataManager : public Ref
{
public:
	
	static SocketDataManager* getInstance();
	CREATE_FUNC(SocketDataManager);
	virtual bool init();

	virtual void update(float dt);

	void pushMsg(std::string str);  /* ���һ�����ݵ������� */
private:
	static SocketDataManager* m_SocketDataManager;

	std::mutex m_mutex;
	CRITICAL_SECTION g_cs;
	std::vector<std::string> m_msgList; /* ������н��յ������� */
};

#endif