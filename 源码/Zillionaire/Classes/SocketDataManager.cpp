#include "SocketDataManager.h"
#include "UFunction.h"
#include "UDefine.h"
#include "GameMapBaseScene.h"

SocketDataManager* SocketDataManager::m_SocketDataManager = NULL;

SocketDataManager* SocketDataManager::getInstance()	//��������
{
	if (m_SocketDataManager == NULL)
	{
		m_SocketDataManager = SocketDataManager::create();
		m_SocketDataManager->retain();
	}

	return m_SocketDataManager;
}

bool SocketDataManager::init()	//��ʼ��
{
	/* ����update���� */
	Director::getInstance()->getScheduler()->schedule(
		schedule_selector(SocketDataManager::update),
		this, 0.108f, false);

	InitializeCriticalSection(&g_cs);
	return true;
}

void SocketDataManager::update(float dt)	//���´�������
{
	m_mutex.lock();

	for (int i = 0; i < m_msgList.size(); i++)
	{
		EnterCriticalSection(&g_cs);

		std::string str = m_msgList[i];
		std::vector<std::string>temp;
		UFunction::splitString(str.c_str(), "-", temp);
		std::string key = temp[0];
		int value = atoi(temp[1].c_str());
		if (key == "PlayerBuyLand")	//���
		{
			String * str = String::createWithFormat("%d-%d", MSG_BUY_LAND_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		}
		if (key == "EnemyBuyLand")	//���
		{
			String * str = String::createWithFormat("%d-%d", MSG_BUY_LAND_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		}
		if (key == "Player1Money")	//�Ʋ�
		{
			String * str = String::createWithFormat("%d-%d", MSG_MONEY_PLAYER1_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_MONEY, str);
		}
		if (key == "Player2Money")	//�Ʋ�
		{
			String * str = String::createWithFormat("%d-%d", MSG_MONEY_PLAYER2_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_MONEY, str);
		}
		if (key == "Player1Strength")	//����
		{
			String * str = String::createWithFormat("%d-%d", MSG_STRENGTH_PLAYER1_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_STRENGTH, str);
		}
		if (key == "Player2Strength")	//����
		{
			String * str = String::createWithFormat("%d-%d", MSG_STRENGTH_PLAYER2_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_STRENGTH, str);
		}
		if (key == "Player1Stock")	//��Ʊ
		{
			String * str = String::createWithFormat("%d-%d-%d-%d", MSG_BUY_PLAYER1_STOCK_TAG, value, atoi(temp[2].c_str()), atoi(temp[3].c_str()));
			NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		}
		if (key == "Player2Stock")	//��Ʊ
		{
			String * str = String::createWithFormat("%d-%d-%d-%d", MSG_BUY_PLAYER2_STOCK_TAG, value, atoi(temp[2].c_str()), atoi(temp[3].c_str()));
			NotificationCenter::getInstance()->postNotification(MSG_STRENGTH, str);
		}
		if (key == "BuyLotteryNumber")	//��Ʊ����
		{
			String * str = String::createWithFormat("%d-%d", MSG_BUY_LOTTERY_NUMBER_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		}
		if (key == "LotteryPublishNumber")	//��������
		{
			String * str = String::createWithFormat("%d-%d", MSG_BUY_LOTTERY_PUBLISH_NUMBER_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		}
		if (key == "BuyShopItemNumber")	//���߱��
		{
			String * str = String::createWithFormat("%d-%d", MSG_BUY_SHOP_ITEM_NUMBER_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		}
		if (key == "UseShopItemNumber")	//ʹ�õ��ߺ���
		{
			String * str = String::createWithFormat("%d-%d", MSG_PACKAGE_USE_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		}
		if (key == "row")	//·����
		{
			GameMapBaseScene::rowVector.clear();
			for (int i = 1; i < temp.size(); i++)
				GameMapBaseScene::rowVector.push_back(atoi(temp[i].c_str()));
		}
		if (key == "col")	//·����
		{
			GameMapBaseScene::colVector.clear();
			for (int i = 1; i < temp.size(); i++)
				GameMapBaseScene::colVector.push_back(atoi(temp[i].c_str()));
			String * str = String::createWithFormat("%d", MSG_GO_TAG);
			NotificationCenter::getInstance()->postNotification(MSG_GO, str);
		}
		if (key == "RandomAskEvent")	//���ʱ��
		{
			String * str = String::createWithFormat("%d-%d-%d-%d", MSG_RANDOM_DO_ASK_EVENT_TAG, atoi(temp[1].c_str()), atoi(temp[2].c_str()), atoi(temp[3].c_str()));
			NotificationCenter::getInstance()->postNotification(MSG_RANDOM_ASK_EVENT, str);
		}
		if (key == "BuyStock")	//���Ʊ
		{
			String * str = String::createWithFormat("%d-%d", MSG_BUY_STOCK_CON_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		}
		if (key == "GameOver")	//���߽�����Ϸ
		{
			String * str = String::createWithFormat("%d-%d-%d", MSG_GAME_OVER_TAG, 0,value);
			NotificationCenter::getInstance()->postNotification(MSG_GAME_OVER, str);
		}
		if (key == "GameOverMoney")	//�Ʋ����������Ϸ
		{
			String * str = String::createWithFormat("%d-%d-%d", MSG_GAME_OVER_TAG,1, value);
			NotificationCenter::getInstance()->postNotification(MSG_GAME_OVER, str);
		}
		if (key == "GameOverStrength")	//�������������Ϸ
		{
			String * str = String::createWithFormat("%d-%d-%d", MSG_GAME_OVER_TAG, 2, value);
			NotificationCenter::getInstance()->postNotification(MSG_GAME_OVER, str);
		}
		if (key == "Rest")	//������Ϣ
		{
			String * str = String::createWithFormat("%d-%d", MSG_GO_REST_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_GO, str);
		}
		LeaveCriticalSection(&g_cs);
	}

	m_msgList.clear();

	m_mutex.unlock();
}

void SocketDataManager::pushMsg(std::string str)	//push��Ϣ
{
	m_mutex.lock();

	m_msgList.push_back(str);

	m_mutex.unlock();
}
