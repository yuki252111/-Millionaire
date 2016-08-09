#include "GameHall.h"
#include "GameMapChoiceScene.h"
#include "GameMapBaseScene.h"
#include "GameMapBeachScene.h"
#include "GameMapSeaScene.h"
#include "GameMapSnowScene.h"

Scene* GameHall::createScene()	//创建场景
{
	auto scene = Scene::create();
	auto layer = GameHall::create();
	scene->addChild(layer);
	return scene;
}
bool GameHall::init()	//初始化数据
{
	if (!Layer::init())
		return false;
	
	InitializeCriticalSection(&g_cs);
	InitializeCriticalSection(&m_cs);
	curpos = -1;
	connect();
	//位置指示符
	sign =  LabelTTF::create();
	sign->setString("ME");
	sign->setAnchorPoint(ccp(0, 0.5));
	sign->setFontSize(15);
	sign->setPosition(Point(0,0));
	sign->setColor(Color3B::BLACK);
	this->addChild(sign,1);
	sign->setVisible(false);
	//加载大厅
	auto UI = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("GameHall.json");
	this->addChild(UI);
	UI->setPosition(Point(270,15));

	for (int i = 1; i <= 18; i++)
	{
		char name[20];
		memset(name, 0, 20);
		sprintf_s(name, "pos%d", i);
		Button* posButton = (Button*)Helper::seekWidgetByName(UI, name);
		posButton->addTouchEventListener(this, toucheventselector(GameHall::onclick));
		posButtons.pushBack(posButton);
	}
	for (int i = 0; i < 18; i++)
	{
		posPoints.push_back(posButtons.at(i)->getPosition());
	}
	for (int i = 0; i < 18; i++)
	{
		Sprite* player = Sprite::create("player.png");
		players.pushBack(player);
		player->setVisible(false);
	}

	for (int i = 0; i < 18; i++)
	{
		this->addChild(players.at(i));
		players.at(i)->setPosition(posPoints[i]);
	}
	return true;
}
void GameHall::onclick(Ref* ref, TouchEventType type)	//点击回调函数
{
	Button* btn = (Button*)ref;
	Point pos = btn->getPosition();
	int index = -1;
	switch (type){
	case TouchEventType::TOUCH_EVENT_BEGAN:break;
	case TouchEventType::TOUCH_EVENT_MOVED:break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		for (int i = 0; i < players.size(); i++)
		{
			if (players.at(i)->getPosition() == pos)
			{
				if (players.at(i)->isVisible())
					return;
				else
				{
					index = i;
					break;
				}
			}
		}
		
		char temp[MAX_LEN];
		memset(temp, 0, MAX_LEN);
		sprintf_s(temp, "Position-%d", index);
		GameMapBaseScene::cSocket.Send(temp, MAX_LEN);
		curpos = index;
		
		sign->setVisible(true);
		sign->setPosition(Point(posPoints[curpos].x-10, posPoints[curpos].y + 20));

		break;
	}
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	}
}
bool GameHall::connect()	//连接服务器
{
	GameMapBaseScene::cSocket.Init();
	GameMapBaseScene::cSocket.Create(AF_INET, SOCK_STREAM, 0);

	bool result = GameMapBaseScene::cSocket.Connect(HOST, PORT);
	int retryTimes = 0;
	while (result == false && retryTimes < 7)	//连接
	{
		result = GameMapBaseScene::cSocket.Connect(HOST, PORT);
		retryTimes++;
		#if(CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
			Sleep(500);
		#else
			usleep(500);
		#endif
	}
	if (result == true)	//成功连接
	{
		std::thread receiveThread = std::thread(&GameHall::receiveData, this);
		receiveThread.detach();

		Director::getInstance()->getScheduler()->schedule(
			schedule_selector(GameHall::update),
			this, 0.108f, false);

		char temp[MAX_LEN];
		memset(temp, 0, MAX_LEN);
		sprintf_s(temp, "SelectedMap-%d", GameMapBaseScene::selectedMap);
		GameMapBaseScene::cSocket.Send(temp, MAX_LEN);

	}
	return result;
}
void GameHall::receiveData()	//接收数据
{
	while (true)
	{
		char receiveBuf[MAX_LEN] = "";
		int iResult = GameMapBaseScene::cSocket.Recv(receiveBuf, MAX_LEN, 0);
		if (iResult <= 0)
			break;

		EnterCriticalSection(&g_cs);
		
			std::vector<std::string>temp;
			temp.clear();
			UFunction::splitString(receiveBuf, " ", temp);
			for (int i = 0; i < temp.size(); i++)
			{
				std::string str = temp[i];
				msg.push_back(str);
			}
		LeaveCriticalSection(&g_cs);
		if (GameMapBaseScene::isStart == true)
			return;
	}
}
void GameHall::update(float dt)	//处理数据
{
	for (int i = 0; i < msg.size(); i++)
	{
		std::string str = msg[i];
		std::vector<std::string>temp;
		UFunction::splitString(str.c_str(), ":", temp);
		std::string key = temp[0];
		if (key == "Poses")
		{
			for (int j = 0; j < players.size(); j++)
				players.at(j)->setVisible(false);
			for (int j = 1; j < temp.size(); j++)
			{
				if (atoi(temp[j].c_str())>=0)
					players.at(atoi(temp[j].c_str()))->setVisible(true);
			}
		}
		if (key == "isStart")
		{
			int arg1 = atoi(temp[1].c_str());
			int arg2 = atoi(temp[2].c_str());
			GameMapBaseScene::isStart = arg1;
			GameMapBaseScene::isTurn = arg2;
			curpos = -1;
			switch (GameMapBaseScene::selectedMap){
			case 0:
				Director::getInstance()->pushScene(TransitionFadeBL::create(2 * ONE_SECOND, GameMapBeachScene::createScene()));
				break;
			case 1:
				Director::getInstance()->pushScene(TransitionFadeBL::create(2 * ONE_SECOND, GameMapSeaScene::createScene()));
				break;
			case 2:
				Director::getInstance()->pushScene(TransitionFadeBL::create(2 * ONE_SECOND, GameMapSnowScene::createScene()));
				break;
			}
			
			this->unschedule(schedule_selector(GameHall::update));
			return;
		}
	}
	msg.clear();

}