#include "SocketDataManager.h"
#include "UFunction.h"
#include "UDefine.h"
#include "GameMapBaseScene.h"

SocketDataManager* SocketDataManager::m_SocketDataManager = NULL;

SocketDataManager* SocketDataManager::getInstance()	//创建单例
{
	if (m_SocketDataManager == NULL)
	{
		m_SocketDataManager = SocketDataManager::create();
		m_SocketDataManager->retain();
	}

	return m_SocketDataManager;
}

bool SocketDataManager::init()	//初始化
{
	/* 调用update函数 */
	Director::getInstance()->getScheduler()->schedule(
		schedule_selector(SocketDataManager::update),
		this, 0.108f, false);

	InitializeCriticalSection(&g_cs);
	return true;
}

void SocketDataManager::update(float dt)	//更新处理数据
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
		if (key == "PlayerBuyLand")	//买地
		{
			String * str = String::createWithFormat("%d-%d", MSG_BUY_LAND_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		}
		if (key == "EnemyBuyLand")	//买地
		{
			String * str = String::createWithFormat("%d-%d", MSG_BUY_LAND_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		}
		if (key == "Player1Money")	//财产
		{
			String * str = String::createWithFormat("%d-%d", MSG_MONEY_PLAYER1_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_MONEY, str);
		}
		if (key == "Player2Money")	//财产
		{
			String * str = String::createWithFormat("%d-%d", MSG_MONEY_PLAYER2_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_MONEY, str);
		}
		if (key == "Player1Strength")	//体力
		{
			String * str = String::createWithFormat("%d-%d", MSG_STRENGTH_PLAYER1_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_STRENGTH, str);
		}
		if (key == "Player2Strength")	//体力
		{
			String * str = String::createWithFormat("%d-%d", MSG_STRENGTH_PLAYER2_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_STRENGTH, str);
		}
		if (key == "Player1Stock")	//股票
		{
			String * str = String::createWithFormat("%d-%d-%d-%d", MSG_BUY_PLAYER1_STOCK_TAG, value, atoi(temp[2].c_str()), atoi(temp[3].c_str()));
			NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		}
		if (key == "Player2Stock")	//股票
		{
			String * str = String::createWithFormat("%d-%d-%d-%d", MSG_BUY_PLAYER2_STOCK_TAG, value, atoi(temp[2].c_str()), atoi(temp[3].c_str()));
			NotificationCenter::getInstance()->postNotification(MSG_STRENGTH, str);
		}
		if (key == "BuyLotteryNumber")	//彩票号码
		{
			String * str = String::createWithFormat("%d-%d", MSG_BUY_LOTTERY_NUMBER_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		}
		if (key == "LotteryPublishNumber")	//开奖号码
		{
			String * str = String::createWithFormat("%d-%d", MSG_BUY_LOTTERY_PUBLISH_NUMBER_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		}
		if (key == "BuyShopItemNumber")	//道具编号
		{
			String * str = String::createWithFormat("%d-%d", MSG_BUY_SHOP_ITEM_NUMBER_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		}
		if (key == "UseShopItemNumber")	//使用道具号码
		{
			String * str = String::createWithFormat("%d-%d", MSG_PACKAGE_USE_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		}
		if (key == "row")	//路径行
		{
			GameMapBaseScene::rowVector.clear();
			for (int i = 1; i < temp.size(); i++)
				GameMapBaseScene::rowVector.push_back(atoi(temp[i].c_str()));
		}
		if (key == "col")	//路径列
		{
			GameMapBaseScene::colVector.clear();
			for (int i = 1; i < temp.size(); i++)
				GameMapBaseScene::colVector.push_back(atoi(temp[i].c_str()));
			String * str = String::createWithFormat("%d", MSG_GO_TAG);
			NotificationCenter::getInstance()->postNotification(MSG_GO, str);
		}
		if (key == "RandomAskEvent")	//随机时间
		{
			String * str = String::createWithFormat("%d-%d-%d-%d", MSG_RANDOM_DO_ASK_EVENT_TAG, atoi(temp[1].c_str()), atoi(temp[2].c_str()), atoi(temp[3].c_str()));
			NotificationCenter::getInstance()->postNotification(MSG_RANDOM_ASK_EVENT, str);
		}
		if (key == "BuyStock")	//买股票
		{
			String * str = String::createWithFormat("%d-%d", MSG_BUY_STOCK_CON_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		}
		if (key == "GameOver")	//断线结束游戏
		{
			String * str = String::createWithFormat("%d-%d-%d", MSG_GAME_OVER_TAG, 0,value);
			NotificationCenter::getInstance()->postNotification(MSG_GAME_OVER, str);
		}
		if (key == "GameOverMoney")	//财产耗完结束游戏
		{
			String * str = String::createWithFormat("%d-%d-%d", MSG_GAME_OVER_TAG,1, value);
			NotificationCenter::getInstance()->postNotification(MSG_GAME_OVER, str);
		}
		if (key == "GameOverStrength")	//体力耗完结束游戏
		{
			String * str = String::createWithFormat("%d-%d-%d", MSG_GAME_OVER_TAG, 2, value);
			NotificationCenter::getInstance()->postNotification(MSG_GAME_OVER, str);
		}
		if (key == "Rest")	//陷阱休息
		{
			String * str = String::createWithFormat("%d-%d", MSG_GO_REST_TAG, value);
			NotificationCenter::getInstance()->postNotification(MSG_GO, str);
		}
		LeaveCriticalSection(&g_cs);
	}

	m_msgList.clear();

	m_mutex.unlock();
}

void SocketDataManager::pushMsg(std::string str)	//push信息
{
	m_mutex.lock();

	m_msgList.push_back(str);

	m_mutex.unlock();
}
