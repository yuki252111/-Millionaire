#include "GameMapBaseScene.h"
#include <thread>
#include <mutex>
#include "SocketDataManager.h"
#include "CocosToast.h"
#include "LineChart.h"

std::mutex mtx;
Vector<Sprite*> GameMapBaseScene::pathMarkVector;
TMXTiledMap* GameMapBaseScene::_map;
TMXLayer* GameMapBaseScene::_land;
TMXLayer* GameMapBaseScene::wayLayer;
Point GameMapBaseScene::shopPoint;
Point GameMapBaseScene::godPoint1;
Point GameMapBaseScene::godPoint2;
Point GameMapBaseScene::trapPoint;
CRITICAL_SECTION GameMapBaseScene::g_ms;
std::vector<int> GameMapBaseScene::rowVector;
std::vector<int> GameMapBaseScene::colVector;
int GameMapBaseScene::selectedMap;
ODSocket GameMapBaseScene::cSocket;

int  GameMapBaseScene::blank_land_tiledID;
int  GameMapBaseScene::strength_30_tiledID;
int  GameMapBaseScene::strength_50_tiledID;
int  GameMapBaseScene::strength_80_tiledID;

int  GameMapBaseScene::randomEvent_tiledID;
int  GameMapBaseScene::lottery_tiledID;
int  GameMapBaseScene::stock_tiledID;
int  GameMapBaseScene::shop_tiledID;

int GameMapBaseScene::player1_building_1_tiledID;
int GameMapBaseScene::player1_building_2_tiledID;
int GameMapBaseScene::player1_building_3_tiledID;

int GameMapBaseScene::player2_building_1_tiledID;
int GameMapBaseScene::player2_building_2_tiledID;
int GameMapBaseScene::player2_building_3_tiledID;

bool GameMapBaseScene::isTurn;
bool GameMapBaseScene::isStart;

std::vector<float> GameMapBaseScene::stock_pointvec1;
std::vector<float> GameMapBaseScene::stock_pointvec2;
std::vector<float> GameMapBaseScene::stock_pointvec3;
std::vector<float> GameMapBaseScene::stock_pointvec4;
std::vector<float> GameMapBaseScene::stock_pointvec5;

GameMapBaseScene::~GameMapBaseScene()	//析构函数
{
	if (SOCKET(cSocket) != NULL)
	{
		shutdown(SOCKET(cSocket), 2);	//关闭socket
		closesocket(SOCKET(cSocket));
		cSocket.Clean();
	}
}

Scene* GameMapBaseScene::createScene()	//创建场景
{
	
	auto scene = Scene::create();

	auto layer = GameMapBaseScene::create();

	scene->addChild(layer);

	return scene;

}

bool GameMapBaseScene::init()	//初始化
{
	if (!Layer::init())
		return false;
	currentPlayer = NULL; move_or_end = 0;
	isPublish = false;
	
	addMap();
	addLand();
	initTiledGrid();
	setWayPassGrid();

	addRightBanner();
	drawTable(2);
	addGoButton();
	addDice();
	addDigiteRoundSprite();

	initPropTiledID();
	initItem();
	addPathMark();
	addPlayer();
	
	refreshRoundDisplay();

	
	initPopupDialoge();
	initLotteryDialoge();
	initLotteryPublishDialoge();
	initShopItemDialoge();
	initPackageDialoge();
	initRandomAskEvent();
	initAudioEffect();

	registerObserver();
	
	InitializeCriticalSection(&g_cs);
	InitializeCriticalSection(&g_ms);
	InitializeCriticalSection(&msg);

	if (isStart == false)
		return false;

	connectServer();
	return true;
}
//--------------------------------------------------
void GameMapBaseScene::addMap() //加載地圖，虛函數
{}
void GameMapBaseScene::addLand() //加載土地
{
	_land = _map->layerNamed("land");
}
void GameMapBaseScene::initTiledGrid() //虛函數，初始化地圖圖標
{}
void GameMapBaseScene::setWayPassGrid() //初始化能夠行走的地位
{
	wayLayer = _map->layerNamed("way");

	Size _mapSize = wayLayer->getLayerSize();
	for (int j = 0; j < _mapSize.width; j++) {
		for (int i = 0; i < _mapSize.height; i++) {
			Sprite* _sp = wayLayer->tileAt(Point(j, i));
			if (_sp)
			{
				float x = _sp->getPositionX();
				float y = _sp->getPositionY();
				int col = x / tiledWidth;
				int row = y / tiledHeight;
				canPassGrid[row][col] = true;
				Point p = _sp->getPosition();
				wayPassPosition.push_back(p);
			}

		}
	}
}
//------------------------------------------------------------------------------
void  GameMapBaseScene::addRightBanner()    //加載右側記錄金錢與體力的精靈
{
}
void GameMapBaseScene::drawTable(int playerNumber) //加載表格
{
	auto s = Director::getInstance()->getWinSize();

	auto draw = DrawNode::create();
	this->addChild(draw);

	for (int i = 0; i<playerNumber; i++)
	{
		draw->drawSegment(Point(tableStartPosition_x, tableStartPosition_y - 2 * i*tableHeight),
			Point(tableStartPosition_x + 3 * tableWidth, tableStartPosition_y - 2 * i*tableHeight), 1,
			Color4F(0, 1, 0, 1));

		draw->drawSegment(Point(tableStartPosition_x, tableStartPosition_y - 2 * (i + 1)*tableHeight),
			Point(tableStartPosition_x + 3 * tableWidth, tableStartPosition_y - 2 * (i + 1)*tableHeight), 1,
			Color4F(0, 1, 0, 1));

		draw->drawSegment(Point(tableStartPosition_x + tableWidth, tableStartPosition_y - tableHeight - 2 * i*tableHeight),
			Point(tableStartPosition_x + 3 * tableWidth, tableStartPosition_y - tableHeight - 2 * i*tableHeight), 1,
			Color4F(0, 1, 0, 1));

		draw->drawSegment(Point(tableStartPosition_x + tableWidth, tableStartPosition_y - 2 * i*tableHeight),
			Point(tableStartPosition_x + tableWidth, tableStartPosition_y - 2 * tableHeight - 2 * i*tableHeight), 1,
			Color4F(0, 1, 0, 1));
	}
}
void GameMapBaseScene::addGoButton() //加載行走圖標按鈕
{
	menu = Menu::create();
	menu->setPosition(CCPointZero);

	MenuItemImage* goButton = MenuItemImage::create("GoNormal.png", "GoPressed.png", this, menu_selector(GameMapBaseScene::goButtonCallFunc));

	goButton->setPosition(ccp(tableStartPosition_x + 2 * tableWidth, tableStartPosition_y - tableHeight * 6));
	menu->addChild(goButton);
	this->addChild(menu);

}
void GameMapBaseScene::goButtonCallFunc(CCObject* pSender) //行走圖標按鈕的囘調函數
{
	if (isTurn)
		currentPlayer = player_me;
	else
		currentPlayer = player_enemy;
	if (isTurn)
	{
		
		stepCount = rand() % 6 + 1;
		NotificationCenter::getInstance()->postNotification(MSG_GO, String::createWithFormat("%d", MSG_GO_HIDE_TAG));
		if (currentPlayer->restDay == 0)
		{
			char temp[MAX_LEN];
			memset(temp, 0, MAX_LEN);
			sprintf_s(temp, "BeginRound-%d", gameRoundCount);
			cSocket.Send(temp, MAX_LEN, 0);

			RouteNavigation::getInstance()->findPath(currentPlayer, stepCount, canPassGrid, tiledRowsCount, tiledColsCount);
			colVector = RouteNavigation::getInstance()->getPathCols();
			rowVector = RouteNavigation::getInstance()->getPathRows();

			std::string rowstr = "row";
			for (int i = 0; i < rowVector.size(); i++)
			{
				rowstr += "-";
				char tmp[20];
				memset(tmp, 0, 20);
				sprintf_s(tmp, "%d", rowVector[i]);
				rowstr += tmp;
			}
			std::string colstr = "col";
			for (int i = 0; i < colVector.size(); i++)
			{
				colstr += "-";
				char tmp[20];
				memset(tmp, 0, 20);
				sprintf_s(tmp, "%d", colVector[i]);
				colstr += tmp;
			}
			std::string str = rowstr + " " + colstr;
			memset(temp, 0, MAX_LEN);
			for (int i = 0; i < str.length(); i++)
				temp[i] = str[i];
			cSocket.Send(temp, MAX_LEN, 0);
		}
		else
		{
			currentPlayer->restDay = currentPlayer->restDay - 1;
			char temp[MAX_LEN];
			memset(temp, 0, MAX_LEN);
			sprintf_s(temp, "Rest-%d",currentPlayer->restDay );
			cSocket.Send(temp, MAX_LEN, 0);
		}
	}
}
void GameMapBaseScene::addDice() //加載色子
{
	dice = Dice::getInstance();
	this->addChild(dice);
}
void GameMapBaseScene::addDigiteRoundSprite()  //加載回合計數器
{
	gameRoundCount = 0;
	auto frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile(ROUND_PLIST, ROUND_PNG);

	char name[20];
	memset(name, 0, 20);
	for (int i = 0; i <= 9; i++)
	{
		sprintf(name, "digital_%d.png", i);
		digiteRoundVector.pushBack(frameCache->getSpriteFrameByName(name));
	}

}
//----------------------------------------------------------
void GameMapBaseScene::initItem()  //初始化道具
{
	//-------------------------------------
	strengthUpInstance = StrengthUp::create();    //加載體力回升動畫
	this->addChild(strengthUpInstance);
	strengthUpInstance->setVisible(false);
	strengthUpInstance->setAnchorPoint(ccp(0, 0.5));
	//-----------------------------------------

	shop = Sprite::create("shop.jpg");            //加載商店圖標
	Point position;
	while (true)
	{
		int shop_pos = rand() % (wayPassPosition.size());
		position = wayPassPosition.at(shop_pos);
		shopPoint = UFunction::GL2map(Point(position.x, position.y + tiledHeight), GameMapBaseScene::_map);
		int endId = wayLayer->getTileGIDAt(shopPoint);
		if (endId != lottery_tiledID&&
			endId != stock_tiledID&&
			endId != randomEvent_tiledID&&
			endId != strength_30_tiledID&&
			endId != strength_50_tiledID&&
			endId != strength_80_tiledID)
			break;
	}
	shop->setVisible(true);
	shop->setPosition(position);
	shop->setAnchorPoint(ccp(0, 0));
	this->addChild(shop);

	//-------------------------------------------
	landGod1 = Sprite::create("landgod.jpg");            //加載土地公圖標
	while (true)
	{
		int god_pos = rand() % (wayPassPosition.size());
		position = wayPassPosition.at(god_pos);
		godPoint1 = UFunction::GL2map(Point(position.x, position.y + tiledHeight), GameMapBaseScene::_map);
		int endId = wayLayer->getTileGIDAt(godPoint1);
		if (endId != lottery_tiledID&&
			endId != stock_tiledID&&
			endId != randomEvent_tiledID&&
			endId != strength_30_tiledID&&
			endId != strength_50_tiledID&&
			endId != strength_80_tiledID&&
			godPoint1!=shopPoint&&
			(_land->getTileGIDAt(UFunction::GL2map(Point(position.x, position.y +  2* tiledHeight), GameMapBaseScene::_map)) == blank_land_tiledID
			|| _land->getTileGIDAt(UFunction::GL2map(Point(position.x, position.y ), GameMapBaseScene::_map)) == blank_land_tiledID
			|| _land->getTileGIDAt(UFunction::GL2map(Point(position.x - tiledWidth, position.y+tiledHeight ), GameMapBaseScene::_map)) == blank_land_tiledID
			|| _land->getTileGIDAt(UFunction::GL2map(Point(position.x+tiledWidth, position.y +tiledHeight), GameMapBaseScene::_map)) == blank_land_tiledID
			)
			)
			break;
	}
	landGod1->setVisible(true);
	landGod1->setPosition(position);
	landGod1->setAnchorPoint(ccp(0, 0));
	this->addChild(landGod1);
	//------------------------------------
	landGod2 = Sprite::create("landgodness.png");            //加載土地婆圖標
	while (true)
	{
		int god_pos = rand() % (wayPassPosition.size());
		position = wayPassPosition.at(god_pos);
		godPoint2 = UFunction::GL2map(Point(position.x, position.y + tiledHeight), GameMapBaseScene::_map);
		int endId = wayLayer->getTileGIDAt(godPoint2);
		if (endId != lottery_tiledID&&
			endId != stock_tiledID&&
			endId != randomEvent_tiledID&&
			endId != strength_30_tiledID&&
			endId != strength_50_tiledID&&
			endId != strength_80_tiledID&&
			godPoint2 != shopPoint&&
			godPoint2 != godPoint1&&
			(_land->getTileGIDAt(UFunction::GL2map(Point(position.x, position.y + tiledHeight), GameMapBaseScene::_map)) == blank_land_tiledID
			|| _land->getTileGIDAt(UFunction::GL2map(Point(position.x, position.y - tiledHeight), GameMapBaseScene::_map)) == blank_land_tiledID
			|| _land->getTileGIDAt(UFunction::GL2map(Point(position.x - tiledWidth, position.y), GameMapBaseScene::_map)) == blank_land_tiledID
			|| _land->getTileGIDAt(UFunction::GL2map(Point(position.x + tiledWidth, position.y), GameMapBaseScene::_map)) == blank_land_tiledID
			)
			)
			break;
	}
	landGod2->setVisible(true);
	landGod2->setPosition(position);
	landGod2->setAnchorPoint(ccp(0, 0));
	this->addChild(landGod2);
	//------------------------------------
	trap = Sprite::create("trap.jpg");            //加載陷阱圖標
	while (true)
	{
		int trap_pos = rand() % (wayPassPosition.size());
		position = wayPassPosition.at(trap_pos);
		trapPoint = UFunction::GL2map(Point(position.x, position.y + tiledHeight), GameMapBaseScene::_map);
		int endId = wayLayer->getTileGIDAt(trapPoint);
		if (endId != lottery_tiledID&&
			endId != stock_tiledID&&
			endId != randomEvent_tiledID&&
			endId != strength_30_tiledID&&
			endId != strength_50_tiledID&&
			endId != strength_80_tiledID
			&&trapPoint!=godPoint1&&trapPoint!=godPoint2&&trapPoint!=shopPoint)
			break;
	}
	trap->setVisible(true);
	trap->setPosition(position);
	trap->setAnchorPoint(ccp(0, 0));
	this->addChild(trap);
	//-----------------------------------------------------------------
	emergencyCar = EmergencyCar::createEmergencyCar();
	emergencyCar->setVisible(false);
	this->addChild(emergencyCar);
}
void GameMapBaseScene::addPathMark()           //加載路徑行走圖標
{
	pathMarkVector.clear();

	Sprite* mark1 = Sprite::create(PATH_MARK_1);
	Sprite* mark2 = Sprite::create(PATH_MARK_2);
	Sprite* mark3 = Sprite::create(PATH_MARK_3);
	Sprite* mark4 = Sprite::create(PATH_MARK_4);
	Sprite* mark5 = Sprite::create(PATH_MARK_5);
	Sprite* mark6 = Sprite::create(PATH_MARK_6);


	mark1->setAnchorPoint(ccp(0, 0));
	mark2->setAnchorPoint(ccp(0, 0));
	mark3->setAnchorPoint(ccp(0, 0));
	mark4->setAnchorPoint(ccp(0, 0));
	mark5->setAnchorPoint(ccp(0, 0));
	mark6->setAnchorPoint(ccp(0, 0));

	mark1->setVisible(false);
	mark2->setVisible(false);
	mark3->setVisible(false);
	mark4->setVisible(false);
	mark5->setVisible(false);
	mark6->setVisible(false);


	this->addChild(mark1);
	this->addChild(mark2);
	this->addChild(mark3);
	this->addChild(mark4);
	this->addChild(mark5);
	this->addChild(mark6);

	pathMarkVector.pushBack(mark1);
	pathMarkVector.pushBack(mark2);
	pathMarkVector.pushBack(mark3);
	pathMarkVector.pushBack(mark4);
	pathMarkVector.pushBack(mark5);
	pathMarkVector.pushBack(mark6);
}
void GameMapBaseScene::drawPathMark(std::vector<int> rowVector, std::vector<int> colVector) //畫出路勁圖標
{
	int stepsCount = rowVector.size() - 1;

	for (int i = 1; i<rowVector.size(); i++)
	{
		pathMarkVector.at(i - 1)->setPosition(ccp(colVector[i] * 32, rowVector[i] * 32));
		pathMarkVector.at(i - 1)->setVisible(true);
	}
}
void GameMapBaseScene::addPlayerInfo()    //加載角色信息到右側信息欄
{
	Sprite* player_me = Sprite::create("Player1.png");
	player_me->setPosition(tableStartPosition_x + tableWidth / 2, tableStartPosition_y - tableHeight);
	this->addChild(player_me);

	//player1财产
	player1_money_label = LabelTTF::create();
	player1_money_label->setString("$");
	player1_money_label->setAnchorPoint(ccp(0, 0.5));
	player1_money_label->setFontSize(25);
	player1_money_label->setPosition(tableStartPosition_x + tableWidth, tableStartPosition_y - tableHeight / 2);
	this->addChild(player1_money_label);
	//player1体力
	player1_strength_label = LabelTTF::create();
	player1_strength_label->setString("+");
	player1_strength_label->setAnchorPoint(ccp(0, 0.5));
	player1_strength_label->setFontSize(28);
	player1_strength_label->setPosition(tableStartPosition_x + tableWidth, tableStartPosition_y - tableHeight / 2 * 3);
	this->addChild(player1_strength_label);

	Sprite* player_enemy1 = Sprite::create("Player2.png");
	player_enemy1->setPosition(tableStartPosition_x + tableWidth / 2, tableStartPosition_y - 3 * tableHeight);
	this->addChild(player_enemy1);
	//player2财产
	player2_money_label = LabelTTF::create();
	player2_money_label->setString("$");
	player2_money_label->setAnchorPoint(ccp(0, 0.5));
	player2_money_label->setFontSize(25);
	player2_money_label->setPosition(tableStartPosition_x + tableWidth, tableStartPosition_y - tableHeight / 2 * 5);
	this->addChild(player2_money_label);
	//player2体力
	player2_strength_label = LabelTTF::create();
	player2_strength_label->setString("+");
	player2_strength_label->setAnchorPoint(ccp(0, 0.5));
	player2_strength_label->setFontSize(28);
	player2_strength_label->setPosition(tableStartPosition_x + tableWidth, tableStartPosition_y - tableHeight / 2 * 7);
	this->addChild(player2_strength_label);
}

void GameMapBaseScene::addPlayer()              //加載角色到地圖上
{
	addPlayerInfo();
	//add player sprite
	struct timeval now;
	gettimeofday(&now, NULL);
	unsigned rand_seed = (unsigned)(now.tv_sec * 1000 + now.tv_usec / 1000);    //都转化为毫秒 
	srand(rand_seed);
	//加载player1
	player1 = Player::createPlayer(PLAYER_1_NAME, PLAYER_1_TAG);

	Point posForPlayer1 = wayPassPosition.at(PLAYER1_POS);
	posForPlayer1.y += tiledHeight;
	player1->setPosition(posForPlayer1);
	player1->setAnchorPoint(ccp(0, 0.5));
	player1->isUsed = false;

	this->addChild(player1);
	//加载player2
	player2 = Player::createPlayer(PLAYER_2_NAME, PLAYER_2_TAG);

	Point posForPlayer2 = wayPassPosition.at(PLAYER2_POS);
	posForPlayer2.y += tiledHeight;
	player2->setPosition(posForPlayer2);
	player2->setAnchorPoint(ccp(0, 0.5));
	player2->isUsed = false;

	this->addChild(player2);
	//设置player1财产
	char money[20];
	memset(money, 0, 20);
	sprintf(money, "$ %d", player1->getMoney());
	player1_money_label->setString(money);
	//设置player1体力
	char strength[20];
	memset(strength, 0, 20);
	sprintf(strength, "+ %d", player1->getStrength());
	player1_strength_label->setString(strength);
	//设置player2财产
	memset(money, 0, 20);
	sprintf(money, "$ %d", player2->getMoney());
	player2_money_label->setString(money);
	//设置player2体力
	memset(strength, 0, 20);
	sprintf(strength, "+ %d", player2->getStrength());
	player2_strength_label->setString(strength);
}

//-----------------------------------------------------------
void GameMapBaseScene::refreshRoundDisplay()  //刷新回合
{

	for (auto it = refreshRoundVector.begin(); it != refreshRoundVector.end(); it++)
	{
		((Sprite*)*it)->setVisible(false);
	}

	refreshRoundVector.clear();
	int count = gameRoundCount;
	Sprite* sp = NULL;

	if (count == 0)
	{
		sp = Sprite::createWithSpriteFrame(digiteRoundVector.at(0));
		this->addChild(sp);
		refreshRoundVector.pushBack(sp);
	}
	while (count != 0)
	{
		sp = Sprite::createWithSpriteFrame(digiteRoundVector.at(count % 10));
		this->addChild(sp);
		refreshRoundVector.pushBack(sp);
		count = count / 10;

	}
	refreshRoundVector.reverse();

	for (int i = 0; i< refreshRoundVector.size(); i++)
	{
		Sprite * sp = refreshRoundVector.at(i);
		sp->setPosition(ccp((tableStartPosition_x + 50) + (i * 25), 50));
		sp->setVisible(true);
	}

	updateStockVec();
	if (currentPlayer != NULL)
		currentPlayer->isUsed = false;
}
//----------------------------------------------------------
void GameMapBaseScene::initPopupDialoge()  //初始化對話彈出框
{
	popupDialoge = PopupLayer::createPopupLayer(DIALOG_BG);

	popupDialoge->setContentSize(CCSizeMake(dialogeWidth, dialogeHeight));
	popupDialoge->setTitle(DIALOG_TITLE);
	popupDialoge->setContentText("", 20, 60, 250);
	popupDialoge->setPopType(NORMAL);
	popupDialoge->setCallbackFunc(this, callfuncN_selector(GameMapBaseScene::buyLandCallback));

	popupDialoge->addButton(BUTTON_BG1, BUTTON_BG3, OK, okTag);
	popupDialoge->addButton(BUTTON_BG2, BUTTON_BG3, CANCEL, cancelTag);
	this->addChild(popupDialoge);
	popupDialoge->setVisible(false);
}
void GameMapBaseScene::initPropTiledID() //初始化道具,虛函數
{}
void GameMapBaseScene::initLotteryDialoge()  //初始化彩票彈出框
{
	lotteryDialoge = PopupLayer::createPopupLayer(DIALOG_BG);

	lotteryDialoge->setContentSize(CCSizeMake(dialogeWidth, dialogeHeight));
	lotteryDialoge->setTitle("Welcome to Lottery!");
	lotteryDialoge->setContentText("", 20, 60, 250);
	lotteryDialoge->setPopType(LOTTERY);
	lotteryDialoge->setCallbackFunc(this, callfuncN_selector(GameMapBaseScene::buyLotteryCallback));

	lotteryDialoge->addButton(BUTTON_BG1, BUTTON_BG3, OK, okTag);
	lotteryDialoge->addButton(BUTTON_BG2, BUTTON_BG3, CANCEL, cancelTag);
	this->addChild(lotteryDialoge);
	lotteryDialoge->setVisible(false);
}
void GameMapBaseScene::buyLotteryCallback(Node* pNode)   //彩票彈出框的囘調函數
{
	if (pNode->getTag() != -1 && pNode->getTag() != cancelTag&&lotteryDialoge->lotteryBought)
	{
		lotteryDialoge->setVisible(false);
		lotteryDialoge->lotteryBought = false;
		UFunction::playEffectMusic(P1_DEYIDEYITIAN, false);

		char temp[MAX_LEN];
		memset(temp, 0, MAX_LEN);
		sprintf_s(temp, "BuyLotteryNumber-%d", pNode->getTag());
		cSocket.Send(temp, MAX_LEN);
	}
	else
	{
		lotteryDialoge->setVisible(false);
		lotteryDialoge->lotteryBought = false;

		char temp[MAX_LEN];
		memset(temp, 0, MAX_LEN);
		sprintf_s(temp, "BuyLotteryNumber-%d",0);
		cSocket.Send(temp, MAX_LEN);
	}
}
void GameMapBaseScene::initLotteryPublishDialoge()  //初始化彩票開獎彈出框
{
	lotteryPublishDialoge = PopupLayer::createPopupLayer(DIALOG_BG);

	lotteryPublishDialoge->setContentSize(CCSizeMake(dialogeWidth, dialogeHeight + 180));
	lotteryPublishDialoge->setTitle("Good Luck to You!");
	lotteryPublishDialoge->setContentText("", 20, 60, 250);
	lotteryPublishDialoge->setPopType(LOTTERY_PUBLISH);
	lotteryPublishDialoge->addPlayer(player1);
	lotteryPublishDialoge->addPlayer(player2);
	lotteryPublishDialoge->setTag(100);
	this->addChild(lotteryPublishDialoge);
	lotteryPublishDialoge->setVisible(false);
}
void GameMapBaseScene::initShopItemDialoge()        //初始化商店道具彈出框
{
	shopItemDialoge = PopupLayer::createPopupLayer(DIALOG_BG);

	shopItemDialoge->setContentSize(CCSizeMake(dialogeWidth, dialogeHeight));
	shopItemDialoge->setTitle("Welcome to shop-buy as you are happy");
	shopItemDialoge->setContentText("", 20, 60, 250);
	shopItemDialoge->setPopType(SHOP);
	shopItemDialoge->setCallbackFunc(this, callfuncN_selector(GameMapBaseScene::buyShopItemCallback));

	shopItemDialoge->addButton(BUTTON_BG1, BUTTON_BG3, OK, okTag);
	shopItemDialoge->addButton(BUTTON_BG2, BUTTON_BG3, CANCEL, cancelTag);
	this->addChild(shopItemDialoge);
	shopItemDialoge->setVisible(false);
}
void GameMapBaseScene::buyShopItemCallback(Node* pNode) //買商店道具的囘調函數
{
	if (pNode->getTag() != -1 && pNode->getTag() != cancelTag&&shopItemDialoge->bought)
	{
		int shopItemMoney = 0;
		for (int i = 0; i < shopItemDialoge->shopItemVector.size(); i++)
		{
			if ((shopItemDialoge->shopItemVector.at(i))->getItemNumber() == pNode->getTag())
			{
				shopItemMoney = (shopItemDialoge->shopItemVector.at(i))->getitemMoney();
				break;
			}
		}
		refreshMoneyLabel(currentPlayer, -shopItemMoney);
		shopItemDialoge->setVisible(false);
		shopItemDialoge->bought = false;

		char temp[MAX_LEN];
		memset(temp, 0, MAX_LEN);
		sprintf_s(temp, "BuyShopItemNumber-%d", pNode->getTag());
		cSocket.Send(temp, MAX_LEN);
	}
	else
	{
		shopItemDialoge->setVisible(false);
		shopItemDialoge->bought = false;

		char temp[MAX_LEN];
		memset(temp, 0, MAX_LEN);
		sprintf_s(temp, "BuyShopItemNumber-%d", 0);
		cSocket.Send(temp, MAX_LEN);
	}
}
void GameMapBaseScene::initPackageDialoge()   //初始化角色包裹彈出框
{
	packageDialoge = PopupLayer::createPopupLayer(DIALOG_BG);

	packageDialoge->setContentSize(CCSizeMake(dialogeWidth, dialogeHeight));
	packageDialoge->setTitle("Your Package!");
	packageDialoge->setContentText("", 20, 60, 250);
	packageDialoge->setPopType(PACKAGE);
	packageDialoge->setCallbackFunc(this, callfuncN_selector(GameMapBaseScene::packageCallback));

	packageDialoge->addButton(BUTTON_BG1, BUTTON_BG3, OK, okTag);
	packageDialoge->addButton(BUTTON_BG2, BUTTON_BG3, CANCEL, cancelTag);
	this->addChild(packageDialoge);
	packageDialoge->setVisible(false);
	itemTag = 0;
}
void GameMapBaseScene::packageCallback(Node* pNode)  //使用道具的囘調函數
{
	if (pNode->getTag() != -1 && pNode->getTag() != cancelTag&&packageDialoge->used)
	{
		packageDialoge->clearPlayerPackageContext();
		packageDialoge->setVisible(false);
		packageDialoge->used = false;

		char temp[MAX_LEN];
		memset(temp, 0, MAX_LEN);
		sprintf_s(temp, "UseShopItemNumber-%d", pNode->getTag());
		cSocket.Send(temp, MAX_LEN);
	}
	else
	{
		packageDialoge->clearPlayerPackageContext();
		packageDialoge->setVisible(false);
		packageDialoge->used = false;
		
		char temp[MAX_LEN];
		memset(temp, 0, MAX_LEN);
		sprintf_s(temp, "UseShopItemNumber-%d",0);
		cSocket.Send(temp, MAX_LEN);
	}
}
void GameMapBaseScene::initRandomAskEvent()            //初始化隨機觸發事件
{
	randomAskEventMap.insert(TAX_REBATE_TAG, String::create(TAX_REBATE));
	randomAskEventMap.insert(TAX_PAY_TAG, String::create(TAX_PAY));
	randomAskEventMap.insert(STRENGTH_LOSS_TAG, String::create(STRENGTH_LOSS));
	randomAskEventMap.insert(STRENGTH_GET_TAG, String::create(STRENGTH_GET));
	randomAskEventMap.insert(INVESTMENT_PROFIT_TAG, String::create(INVESTMENT_PROFIT));
	randomAskEventMap.insert(INVESTMENT_LOSS_TAG, String::create(INVESTMENT_LOSS));
}
void GameMapBaseScene::initAudioEffect()
{
	bgMusicVector.push_back(BG01_MP3);
	bgMusicVector.push_back(BG02_MP3);
	bgMusicVector.push_back(BG03_MP3);

	for (int i = 0; i < bgMusicVector.size(); i++)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(bgMusicVector.at(i));
	}
	this->scheduleOnce(schedule_selector(GameMapBaseScene::playerBgMusic), 1.0f);
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.75f);
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.25f);

	//交过路费声音
	player1EffectVec_1.pushBack(String::create(P1_Speaking_00435));
	player1EffectVec_1.pushBack(String::create(P1_Speaking_00461));
	player1EffectVec_1.pushBack(String::create(P1_Speaking_00475));
	player1EffectVec_1.pushBack(String::create(P1_Speaking_01060));
	player1EffectVec_1.pushBack(String::create(P1_Speaking_001062));
	//抢夺别人地块
	player1EffectVec_2.pushBack(String::create(P1_Speaking_00429));
	//房屋被抢夺
	player1EffectVec_3.pushBack(String::create(P1_Speaking_00430));
	player1EffectVec_3.pushBack(String::create(P1_Speaking_00464));
	player1EffectVec_3.pushBack(String::create(P1_Speaking_00469));
	player1EffectVec_3.pushBack(String::create(P1_Speaking_00470));
	player1EffectVec_3.pushBack(String::create(P1_Speaking_00476));
	//摧毁别人房屋
	player1EffectVec_4.pushBack(String::create(P1_Speaking_00433));
	player1EffectVec_4.pushBack(String::create(P1_Speaking_00437));

	//房屋被摧毁
	player1EffectVec_5.pushBack(String::create(P1_Speaking_00462));
	player1EffectVec_5.pushBack(String::create(P1_Speaking_00463));
	player1EffectVec_5.pushBack(String::create(P1_Speaking_00466));
	player1EffectVec_5.pushBack(String::create(P1_Speaking_00468));
	player1EffectVec_5.pushBack(String::create(P1_Speaking_00474));
	player1EffectVec_5.pushBack(String::create(P1_Speaking_001061));
	
	player1EffectVec_6.pushBack(String::create(P1_Speaking_00449));
	player1EffectVec_6.pushBack(String::create(P1_Speaking_01054));
	player1EffectVec_6.pushBack(String::create(P1_Speaking_01055));
	player1EffectVec_6.pushBack(String::create(P1_Speaking_001071));
	//看到别人住院
	player1EffectVec_7.pushBack(String::create(P1_Speaking_001073));
	//收取过路费
	player1EffectVec_8.pushBack(String::create(P1_Speaking_00453));
	player1EffectVec_8.pushBack(String::create(P1_Speaking_01059));
	player1EffectVec_8.pushBack(String::create(P1_Speaking_01057));
	//升级房子
	player1EffectVec_9.pushBack(String::create(P1_Speaking_01051));
	//不交过路费
	player1EffectVec_10.pushBack(String::create(P1_Speaking_00446));
	player1EffectVec_10.pushBack(String::create(P1_Speaking_00477));
	//买地
	player1EffectVec_11.pushBack(String::create(P1_Speaking_00458));

	
	player1EffectVec_12.pushBack(String::create(P1_Speaking_01052));
	player1EffectVec_12.pushBack(String::create(P1_Speaking_001063));
	//对方被罚收税
	player1EffectVec_13.pushBack(String::create(P1_Speaking_00452));


	//交过路费声音0-6
	player2EffectVec_1.pushBack(String::create(P2_SPEAKING01));
	player2EffectVec_1.pushBack(String::create(P2_QISIWOLE));
	player2EffectVec_1.pushBack(String::create(P2_XINHAOKONGA));
	player2EffectVec_1.pushBack(String::create(P2_BUHUIBA));
	player2EffectVec_1.pushBack(String::create(P2_PAYHIGH));
	player2EffectVec_1.pushBack(String::create(P2_QIANGQIANA));
	player2EffectVec_1.pushBack(String::create(P2_HEBAOCHUXIE));

	//抢夺别人地块7-10
	player2EffectVec_2.pushBack(String::create(P2_BIEGUAIWO));
	player2EffectVec_2.pushBack(String::create(P2_SPEAKING02));
	player2EffectVec_2.pushBack(String::create(P2_TIGER));
	player2EffectVec_2.pushBack(String::create(P2_NIDEJIUSHODE));
	//房屋被抢夺11-14
	player2EffectVec_3.pushBack(String::create(P2_ZHENMIANMU));
	player2EffectVec_3.pushBack(String::create(P2_WODEDIQI));
	player2EffectVec_3.pushBack(String::create(P2_HAOQIFU));
	player2EffectVec_3.pushBack(String::create(P2_WANGFA));
	//摧毁别人房屋15-18
	player2EffectVec_4.pushBack(String::create(P2_NIGAIWOCHAI));
	player2EffectVec_4.pushBack(String::create(P2_KANWODE));
	player2EffectVec_4.pushBack(String::create(P2_HAIRENLE));
	player2EffectVec_4.pushBack(String::create(P2_BAOCHOU));
	//房屋被摧毁19-22
	player2EffectVec_5.pushBack(String::create(P2_WODEYANGFANG));
	player2EffectVec_5.pushBack(String::create(P2_QIFURENJIA));
	player2EffectVec_5.pushBack(String::create(P2_SHAQIANDAO));
	player2EffectVec_5.pushBack(String::create(P2_LIANXIANGXIYU));
	player2EffectVec_5.pushBack(String::create(P2_HAOJIUGAIHAO));
	
	player2EffectVec_6.pushBack(String::create(P2_YUNQICHA));
	player2EffectVec_6.pushBack(String::create(P2_HAIRENJING));
	player2EffectVec_6.pushBack(String::create(P2_XIAOHUANG));
	player2EffectVec_6.pushBack(String::create(P2_YISHENG));
	//看到别人住院27-28
	player2EffectVec_7.pushBack(String::create(P2_DUOXIUXI));
	player2EffectVec_7.pushBack(String::create(P2_XIUXIJITIAN));
	//收取过路费29-34
	player2EffectVec_8.pushBack(String::create(P2_RENBUWEIJI));
	player2EffectVec_8.pushBack(String::create(P2_XIAOQI));
	player2EffectVec_8.pushBack(String::create(P2_RONGXING));
	player2EffectVec_8.pushBack(String::create(P2_MANYI));
	player2EffectVec_8.pushBack(String::create(P2_XIAOFUPO));
	player2EffectVec_8.pushBack(String::create(P2_DUOGEI));
	//升级房子35-36
	player2EffectVec_9.pushBack(String::create(P2_HIGHER));
	player2EffectVec_9.pushBack(String::create(P2_WANZHANGGAOLOU));
	//不交过路费37-38
	player2EffectVec_10.pushBack(String::create(P2_NOTPAY));
	player2EffectVec_10.pushBack(String::create(P2_YIMAOBUGEI));
	//买地39-42
	player2EffectVec_11.pushBack(String::create(P2_BUYIT));
	player2EffectVec_11.pushBack(String::create(P2_HAODEKAISHI));
	player2EffectVec_11.pushBack(String::create(P2_RANGNIZHU));
	player2EffectVec_11.pushBack(String::create(P2_MAIWOBA));
	
	player2EffectVec_12.pushBack(String::create(P2_DEYIDEYITIAN));
	player2EffectVec_12.pushBack(String::create(P2_HENGCAI));
	//对方被罚收税45-48
	player2EffectVec_13.pushBack(String::create(P2_TOUSHUI));
	player2EffectVec_13.pushBack(String::create(P2_FALVZHICAI));
	player2EffectVec_13.pushBack(String::create(P2_GUOKU));
	player2EffectVec_13.pushBack(String::create(P2_NASHUI));
}
void GameMapBaseScene::playerBgMusic(float dt)	//播放背景音乐
{
	if (!CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		UFunction::playBgMusic(bgMusicVector.at(rand() % bgMusicVector.size()), true);
	}
}
//------------------------------------------------------------
void GameMapBaseScene::registerObserver()                   //消息註冊函數
{
	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(GameMapBaseScene::dealMsg),
		MSG_GO,
		NULL);

	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(GameMapBaseScene::dealMsg),
		MSG_BUY,
		NULL);
	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(GameMapBaseScene::dealMsg),
		MSG_PAY_TOLLS,
		NULL);

	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(GameMapBaseScene::dealMsg),
		MSG_MONEY,
		NULL);
	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(GameMapBaseScene::dealMsg),
		MSG_RANDOM_ASK_EVENT,
		NULL);
	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(GameMapBaseScene::dealMsg),
		MSG_STRENGTH,
		NULL);
	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(GameMapBaseScene::dealMsg),
		MSG_GAME_OVER,
		NULL);
}
void GameMapBaseScene::dealMsg(Object* data)              //消息處理函數
{
	EnterCriticalSection(&msg);

	String * srcDate = (String*)data;
	std::vector<std::string>messageVector;
	messageVector.clear();
	UFunction::splitString(srcDate->getCString(), "-", messageVector);
	
	LeaveCriticalSection(&msg);

	int retMsgType = atoi(messageVector[0].c_str());
	Vector<Node*> vecMenuItem = menu->getChildren();
	if (isTurn)
		currentPlayer = player_me;
	else
		currentPlayer = player_enemy;

	switch (retMsgType){
	case MSG_MONEY_PLAYER1_TAG:	//player1财产处理
	{
		std::string money = messageVector[1];
		player1->setMoney(atoi(money.c_str()));
		player1_money_label->setString(money.c_str());
		if (player1->getMoney() < 1000)
		{
			char temp[MAX_LEN];
			memset(temp, 0, MAX_LEN);
			sprintf_s(temp, "GameOverMoney-%d", PLAYER_1_TAG);
			cSocket.Send(temp, MAX_LEN);
		}
		break;
	}
	case MSG_MONEY_PLAYER2_TAG:	//player2财产处理
	{
		std::string money = messageVector[1];
		player2->setMoney(atoi(money.c_str()));
		player2_money_label->setString(money.c_str());
		if (player2->getMoney() < 1000)
		{
			char temp[MAX_LEN];
			memset(temp, 0, MAX_LEN);
			sprintf_s(temp, "GameOverMoney-%d", PLAYER_2_TAG);
			cSocket.Send(temp, MAX_LEN);
		}
		break;
	}
	case MSG_STRENGTH_PLAYER1_TAG:	//player1体力处理
	{
		std::string strength = messageVector[1];
		player1->setStrength(atoi(strength.c_str()));
		player1_strength_label->setString(strength.c_str());
		if (player1->getStrength() <= 0)
		{
			if (isTurn)
			{
				char temp[MAX_LEN];
				memset(temp, 0, MAX_LEN);
				sprintf_s(temp, "GameOverStrength-%d", PLAYER_1_TAG);
				cSocket.Send(temp, MAX_LEN);
			}
		}
		break;
	}
	case MSG_STRENGTH_PLAYER2_TAG://player2体力处理
	{
		std::string strength = messageVector[1];
		player2->setStrength(atoi(strength.c_str()));
		player2_strength_label->setString(strength.c_str());
		if (player2->getStrength() <= 0)
		{
			if (isTurn)
			{
				char temp[MAX_LEN];
				memset(temp, 0, MAX_LEN);
				sprintf_s(temp, "GameOverStrength-%d", PLAYER_2_TAG);
				cSocket.Send(temp, MAX_LEN);
			}
		}
		break;
	}
	case MSG_BUY_PLAYER1_STOCK_TAG://player1股票处理
	{
		int index = atoi(messageVector[1].c_str());
		int price = atoi(messageVector[2].c_str());
		int number = atoi(messageVector[3].c_str());
		player1->stockMap.at(index)->setMakedealprice(price);
		player1->stockMap.at(index)->setStoreNumber(number);
		break;
	}
	case MSG_BUY_PLAYER2_STOCK_TAG://player2股票处理
	{
		int index = atoi(messageVector[1].c_str());
		int price = atoi(messageVector[2].c_str());
		int number = atoi(messageVector[3].c_str());
		player2->stockMap.at(index)->setMakedealprice(price);
		player2->stockMap.at(index)->setStoreNumber(number);
		break;
	}
	case MSG_GO_TAG://player行走处理
	{
		EnterCriticalSection(&g_ms);
		currentPlayer->startGo(rowVector, colVector);
		break;
	}
	case MSG_GO_CON_TAG://player继续行走
	{
		if (isTurn)
		{
			stepCount = atoi(messageVector[1].c_str());

			RouteNavigation::getInstance()->findPath(currentPlayer, stepCount, canPassGrid, tiledRowsCount, tiledColsCount);
			colVector = RouteNavigation::getInstance()->getPathCols();
			rowVector = RouteNavigation::getInstance()->getPathRows();

			char temp[MAX_LEN];
			memset(temp, 0, MAX_LEN);

			std::string rowstr = "row";
			for (int i = 0; i < rowVector.size(); i++)
			{
				rowstr += "-";
				char tmp[20];
				memset(tmp, 0, 20);
				sprintf_s(tmp, "%d", rowVector[i]);
				rowstr += tmp;
			}
			std::string colstr = "col";
			for (int i = 0; i < colVector.size(); i++)
			{
				colstr += "-";
				char tmp[20];
				memset(tmp, 0, 20);
				sprintf_s(tmp, "%d", colVector[i]);
				colstr += tmp;
			}
			std::string str = rowstr + " " + colstr;
			memset(temp, 0, MAX_LEN);
			for (int i = 0; i < str.length(); i++)
				temp[i] = str[i];
			cSocket.Send(temp, MAX_LEN, 0);
		}
		break;
	}
	case MSG_GO_TRAP_TAG://陷阱
	{
		int x = atof(messageVector[1].c_str());
		int y = atof(messageVector[2].c_str());
		int playerTag = atoi(messageVector[3].c_str());
		if (playerTag == PLAYER_1_TAG)
		{
			UFunction::playEffectMusicRandom(player1EffectVec_6, false);
			player1->restDay = TRAP_DAY;
		}
		else
		{
			UFunction::playEffectMusicRandom(player2EffectVec_6, false);
			player2->restDay = TRAP_DAY;
		}
		emergencyCar->setVisible(true);
		int distance = tableStartPosition_x - currentPlayer->getPosition().x;
		emergencyCar->setPosition(currentPlayer->getPosition() + ccp(distance, 0));

		MoveBy* moveBy = MoveBy::create(1.0f, ccp(-distance, 0));
		MoveBy* moveBy2 = MoveBy::create(0.5f, ccp(-60, 0));
		Repeat* repeate = Repeat::create(emergencyCar->getCar_go_anmi(), 5);
		Repeat* repeate2 = Repeat::create(emergencyCar->getCar_stop_anmi(), 1);

		Sequence* spawnAction = Sequence::create(Spawn::create(moveBy, repeate, NULL), Spawn::create(moveBy2, repeate2, NULL), CallFunc::create(CC_CALLBACK_0(GameMapBaseScene::endCarGoAgain, this)), NULL);
		spawnAction->retain();
		emergencyCar->runAction(spawnAction);
		break;
	}
	case MSG_GO_REST_TAG://休息
	{
		int value = atoi(messageVector[1].c_str());
		currentPlayer->restDay = value;
		if (value == 0)
			currentPlayer->setVisible(true);
		CocosToast::createToast(this, String::createWithFormat("Need Rest %d Day", value)->getCString(), 4 * ONE_SECOND, currentPlayer->getPosition());
		NotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUY, String::createWithFormat("%d", MSG_BUY_LOTTERY_PUBLISH_TAG));
		break;
	}
	case MSG_STRENGTH_UP30_TAG://体力会升
	{
		UFunction::playEffectMusic(P1_tilihuisheng, false);
		int playerTag = atoi(messageVector[3].c_str());
		move_or_end = atoi(messageVector[4].c_str());

		strengthUpInstance->setVisible(true);
		strengthUpInstance->setPosition(currentPlayer->getPosition() - ccp(5, 0));
		Sequence* action = Sequence::create(strengthUpInstance->getNormal_anmi(),
			CallFunc::create([this](){
			strengthUpInstance->setVisible(false);
			CocosToast::createToast(this, STRENGTH_UP_30, 4 * ONE_SECOND, currentPlayer->getPosition());

		}
			), CallFunc::create([&](){
			this->scheduleOnce(schedule_selector(GameMapBaseScene::Info), 4 * ONE_SECOND);
		}), NULL);

		strengthUpInstance->runAction(action);
		if (isTurn)
			refreshStrengthLabel(currentPlayer, 30);
		break;
	}
	case MSG_STRENGTH_UP50_TAG://体力会升
	{
		UFunction::playEffectMusic(P1_tilihuisheng, false);
		int playerTag = atoi(messageVector[3].c_str());
		move_or_end = atoi(messageVector[4].c_str());


		strengthUpInstance->setVisible(true);
		strengthUpInstance->setPosition(currentPlayer->getPosition() - ccp(5, 0));
		Sequence* action = Sequence::create(strengthUpInstance->getNormal_anmi(),
			CallFunc::create([this](){
			strengthUpInstance->setVisible(false);
			CocosToast::createToast(this, STRENGTH_UP_50, 4 * ONE_SECOND, currentPlayer->getPosition());
		}
			), CallFunc::create([&](){
			this->scheduleOnce(schedule_selector(GameMapBaseScene::Info), 4 * ONE_SECOND);
		}), NULL);

		strengthUpInstance->runAction(action);
		if (isTurn)
			refreshStrengthLabel(currentPlayer, 50);
		break;
	}
	case MSG_STRENGTH_UP80_TAG://体力会升
	{
		UFunction::playEffectMusic(P1_tilihuisheng, false);
		int playerTag = atoi(messageVector[3].c_str());
		move_or_end = atoi(messageVector[4].c_str());

		strengthUpInstance->setVisible(true);
		strengthUpInstance->setPosition(currentPlayer->getPosition() - ccp(5, 0));
		Sequence* action = Sequence::create(strengthUpInstance->getNormal_anmi(),
			CallFunc::create([this](){
			strengthUpInstance->setVisible(false);
			CocosToast::createToast(this, STRENGTH_UP_80, 4 * ONE_SECOND, currentPlayer->getPosition());
		}
			), CallFunc::create([&](){
			this->scheduleOnce(schedule_selector(GameMapBaseScene::Info), 4 * ONE_SECOND);
		}), NULL);

		strengthUpInstance->runAction(action);
		if (isTurn)
			refreshStrengthLabel(currentPlayer, 80);
		break;
	}
	case MSG_BUY_GOD_TAG://土地公，土地婆
	{
		buy_land_x = atof(messageVector[1].c_str());
		buy_land_y = atof(messageVector[2].c_str());
		int playerTag = atoi(messageVector[3].c_str());
		int titleId = atoi(messageVector[4].c_str());
		if (playerTag == PLAYER_1_TAG && (titleId == GameMapBaseScene::blank_land_tiledID ||
			titleId == GameMapBaseScene::player2_building_1_tiledID || titleId == GameMapBaseScene::player2_building_2_tiledID ||
			titleId == GameMapBaseScene::player2_building_3_tiledID))
		{
			_land->setTileGID(player1_building_1_tiledID, ccp(buy_land_x, buy_land_y));
			UFunction::playEffectMusicRandom(player1EffectVec_2, false);
			this->scheduleOnce(schedule_selector(GameMapBaseScene::InfoLotteryPublish), 1.0f);
		}
		else if (playerTag == PLAYER_2_TAG && (titleId == GameMapBaseScene::blank_land_tiledID ||
			titleId == GameMapBaseScene::player1_building_1_tiledID || titleId == GameMapBaseScene::player1_building_2_tiledID ||
			titleId == GameMapBaseScene::player1_building_3_tiledID))
		{
			_land->setTileGID(player2_building_1_tiledID, ccp(buy_land_x, buy_land_y));
			UFunction::playEffectMusicRandom(player2EffectVec_2, false);
			this->scheduleOnce(schedule_selector(GameMapBaseScene::InfoLotteryPublish), 1.0f);
		}
		else
			this->scheduleOnce(schedule_selector(GameMapBaseScene::InfoEnd), 1.0f);
		break;
	}
	case MSG_BUY_LOTTERY_TAG://买彩票
	{
		if (isTurn)
		{
			lotteryDialoge->setVisible(true);
			UFunction::playEffectMusic(P1_select_lottery, false);
		}
		int tag = atoi(messageVector[4].c_str());
		move_or_end = tag;
		break;
	}
	case MSG_BUY_LOTTERY_NUMBER_TAG://买彩票号码
	{
		int tag = atoi(messageVector[1].c_str());
		if (tag!=0)
		{ 
			currentPlayer->addLottery(tag);
			lotteryDialoge->setHasSelectedLotteryNumber(tag);
			refreshMoneyLabel(currentPlayer, -BUY_LOTTERY_MONEY);
			CocosToast::createToast(this,String::createWithFormat("Buy Lottery %d",tag)->_string, 4 * ONE_SECOND, currentPlayer->getPosition());
		}
		if (move_or_end == MSG_GO_PASS_TAG)
		{
			String * str = String::createWithFormat("%d", MSG_CONTROLLER_PASS_TAG);
			NotificationCenter::getInstance()->postNotification(MSG_CONTROLLER, str);
		}
		else
		{
			this->scheduleOnce(schedule_selector(GameMapBaseScene::InfoEnd), 1.0f);

		}
		break;
	}
	case MSG_BUY_LOTTERY_PUBLISH_TAG://开彩票
	{
		if (gameRoundCount != 0 && gameRoundCount % LOTTERY_TIME_INTERVAL == 0 && isPublish == false)
		{
			if (isTurn)
			{
				int lott = rand() % 30 + 1;
				char temp[MAX_LEN];
				memset(temp, 0, MAX_LEN);
				sprintf_s(temp, "LotteryPublishNumber-%d", lott);
				cSocket.Send(temp, MAX_LEN);
			}
		}
		else
			NotificationCenter::getInstance()->postNotification(MSG_GO, String::createWithFormat("%d", MSG_GO_SHOW_TAG));
		break;
	}
	case MSG_BUY_LOTTERY_PUBLISH_NUMBER_TAG: //开彩票号码
	{
		lotteryPublishDialoge->lott = atoi(messageVector.at(1).c_str());
		UFunction::playEffectMusic(MASHANGKAIJIANG, false);
		lotteryPublishDialoge->setVisible(true);
		lotteryPublishDialoge->addPlayerLottery();
		lotteryPublishDialoge->runLotteryAnim();

		lotteryDialoge->selectedClear();
		isPublish = true;
		break;
	}
	case MSG_BUY_LOTTERY_WIN_TAG://彩票中奖
	{
		refreshMoneyLabel(player1, 0);
		refreshMoneyLabel(player2, 0);
		NotificationCenter::getInstance()->postNotification(MSG_GO, String::createWithFormat("%d", MSG_GO_SHOW_TAG));
		break;
	}
	case MSG_BUY_SHOP_ITEM_TAG://买道具
	{
		if (isTurn)
			shopItemDialoge->setVisible(true);

		int tag = atoi(messageVector[4].c_str());
		move_or_end = tag;
		break;
	}
	case MSG_BUY_SHOP_ITEM_NUMBER_TAG://买道具编号
	{
		int tag = atoi(messageVector[1].c_str());
		if (tag != 0 && tag != -1)
		{
			currentPlayer->addShopItem(tag);
			CocosToast::createToast(this, String::createWithFormat("Buy Item %d", tag)->_string, 4 * ONE_SECOND, currentPlayer->getPosition());
		}
		if (move_or_end == MSG_GO_PASS_TAG)
		{
			String * str = String::createWithFormat("%d", MSG_CONTROLLER_PASS_TAG);
			NotificationCenter::getInstance()->postNotification(MSG_CONTROLLER, str);
		}
		else
		{
			this->scheduleOnce(schedule_selector(GameMapBaseScene::InfoEnd), 1.0f);

		}
		break;
	}
	case MSG_PACKAGE_TAG://包裹
	{
			if (currentPlayer->isUsed == false && currentPlayer->shopItemMap.size() != 0)
			{
				itemTag = 0;
				if (isTurn)
				{
					packageDialoge->setVisible(true);
					packageDialoge->setPlayerPackageContext(currentPlayer);
				}
			}
			else
				this->scheduleOnce(schedule_selector(GameMapBaseScene::InfoController), 0.0f);
		break;
	}
	case MSG_PACKAGE_USE_TAG://用道具
	{
		int selectedTag = atoi(messageVector[1].c_str());
		itemTag = selectedTag;
		if (selectedTag != -1 && selectedTag != 0)
		{
			currentPlayer->isUsed = true;
			std::map<int, int>::iterator iter = currentPlayer->shopItemMap.begin();
			while (iter != currentPlayer->shopItemMap.end())
			{
				if (iter->first == selectedTag)
				{
					iter->second = iter->second - 1;
					if (iter->second == 0)
						currentPlayer->shopItemMap.erase(iter);
					break;
				}
				iter++;
			}
			CocosToast::createToast(this, String::createWithFormat("Use Item %d", selectedTag)->_string, 4 * ONE_SECOND, currentPlayer->getPosition());
			if (selectedTag >= 1 && selectedTag <= 6)
			{
				NotificationCenter::sharedNotificationCenter()->postNotification(MSG_GO, String::createWithFormat("%d-%d", MSG_GO_CON_TAG, selectedTag));
			}
			else
				this->scheduleOnce(schedule_selector(GameMapBaseScene::InfoController), 1.0f);
		}
		else
			this->scheduleOnce(schedule_selector(GameMapBaseScene::InfoController), 1.0f);
		break;
	}
	case MSG_BUY_STOCK_TAG://买股票
	{
		move_or_end = atoi(messageVector[4].c_str());
		if (isTurn)
		{
			UFunction::playEffectMusic(P1_STOCK, false);
			auto lineView = LineChart::createChart(currentPlayer);
			lineView->setPosition(Point(0, 0));
			lineView->moveTag = move_or_end;

			this->addChild(lineView);
		}
		
		break;
	}
	case MSG_BUY_STOCK_DISMISS_TAG://return
	{
		refreshStrengthLabel(player1, 0);
		refreshStrengthLabel(player2, 0);
		refreshStock(player1, 0, 0);
		refreshStock(player2, 0, 0);

		char temp[MAX_LEN];
		memset(temp, 0, MAX_LEN);
		sprintf_s(temp, "BuyStock-%d", move_or_end);
		cSocket.Send(temp, MAX_LEN);

		refreshMoneyLabel(player1, 0);
		refreshMoneyLabel(player2, 0);

		break;
	}
	case MSG_BUY_STOCK_CON_TAG://中途买股票
	{
		move_or_end = atoi(messageVector[1].c_str());
		if (move_or_end == MSG_GO_PASS_TAG)
		{
			NotificationCenter::getInstance()->postNotification(MSG_CONTROLLER, String::createWithFormat("%d", MSG_CONTROLLER_PASS_TAG));

		}
		else if (move_or_end == MSG_GO_END_TAG)
		{
			NotificationCenter::getInstance()->postNotification(MSG_CONTROLLER, String::createWithFormat("%d", MSG_CONTROLLER_AROUND_TAG));
		}
		break;
	}
	case MSG_BUY_NOTHING_TAG://什么都不买
	{
		LeaveCriticalSection(&g_ms);

		String * str = String::createWithFormat("%d", MSG_PACKAGE_TAG);
		NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		break;
	}
	case MSG_BUY_BLANK_DIALOG://买地
	{
			buy_land_x = atof(messageVector[1].c_str());
			buy_land_y = atof(messageVector[2].c_str());
			int playerTag = atoi(messageVector[3].c_str());
			if (itemTag==7)
			{
				_land->setTileGID(blank_land_tiledID, ccp(buy_land_x, buy_land_y));
				itemTag = 0;
				this->scheduleOnce(schedule_selector(GameMapBaseScene::InfoLotteryPublish), 1.0f);
			}
			else
			{
				if (isTurn)
					showBuyLandDialoge(MSG_BUY_BLANK_TAG, playerTag);
				popupDialoge->setDataTag(MSG_BUY_BLANK_TAG);
				popupDialoge->setPlayerTag(playerTag);
			}
		break;
	}
	case MSG_BUY_LAND_1_DIALOG://升级土地
	{
			buy_land_x = atof(messageVector[1].c_str());
			buy_land_y = atof(messageVector[2].c_str());
			int playerTag = atoi(messageVector[3].c_str());
			if (itemTag == 7)
			{
				int endId =_land->getTileGIDAt(ccp(buy_land_x,buy_land_y));
				_land->setTileGID(blank_land_tiledID, ccp(buy_land_x, buy_land_y));
				itemTag = 0;
				if (endId == player1_building_1_tiledID)
					UFunction::playEffectMusicRandom(player1EffectVec_5, false);
				if (endId == player2_building_1_tiledID)
					UFunction::playEffectMusicRandom(player2EffectVec_5, false);
				this->scheduleOnce(schedule_selector(GameMapBaseScene::InfoLotteryPublish), 1.0f);
			}
			else
			{
				if (isTurn)
					showBuyLandDialoge(MSG_BUY_LAND_1_TAG, playerTag);
				popupDialoge->setDataTag(MSG_BUY_LAND_1_TAG);
				popupDialoge->setPlayerTag(playerTag);
			}
		break;
	}
	case MSG_BUY_LAND_2_DIALOG://升级土地
	{
			buy_land_x = atof(messageVector[1].c_str());
			buy_land_y = atof(messageVector[2].c_str());
			int playerTag = atoi(messageVector[3].c_str());
			if (itemTag == 7)
			{
				int endId = _land->getTileGIDAt(ccp(buy_land_x, buy_land_y));
				_land->setTileGID(blank_land_tiledID, ccp(buy_land_x, buy_land_y));
				itemTag = 0;
				if (endId == player1_building_2_tiledID)
					UFunction::playEffectMusicRandom(player1EffectVec_5, false);
				if (endId == player2_building_2_tiledID)
					UFunction::playEffectMusicRandom(player2EffectVec_5, false);
				this->scheduleOnce(schedule_selector(GameMapBaseScene::InfoLotteryPublish), 1.0f);
			}
			else
			{
				if (isTurn)
					showBuyLandDialoge(MSG_BUY_LAND_2_TAG, playerTag);
				popupDialoge->setDataTag(MSG_BUY_LAND_2_TAG);
				popupDialoge->setPlayerTag(playerTag);
			}
		break;
	}
	case MSG_BUY_LAND_TAG://买土地
	{
		int value = atoi(messageVector[1].c_str());
		if (value == 1)
		{
			int playerTag = popupDialoge->getPlayerTag();
			int money = 0;
			switch (popupDialoge->getDataTag()){
			case MSG_BUY_BLANK_TAG:
			{
				money = LAND_BLANK_MONEY;
				if (playerTag == PLAYER_1_TAG)
				{
					_land->setTileGID(player1_building_1_tiledID, ccp(buy_land_x, buy_land_y));
					refreshMoneyLabel(player1, -money);
					UFunction::playEffectMusicRandom(player1EffectVec_11, false);
				}
				else
				{
					_land->setTileGID(player2_building_1_tiledID, ccp(buy_land_x, buy_land_y));
					refreshMoneyLabel(player2, -money);
					UFunction::playEffectMusicRandom(player2EffectVec_11, false);
				}
				break;
			}
			case MSG_BUY_LAND_1_TAG:
			{
				money = LAND_LEVEL_1_MONEY;
				if (playerTag == PLAYER_1_TAG)
				{
					_land->setTileGID(player1_building_2_tiledID, ccp(buy_land_x, buy_land_y));
					refreshMoneyLabel(player1, -money);
					UFunction::playEffectMusicRandom(player1EffectVec_9, false);
				}
				else
				{
					_land->setTileGID(player2_building_2_tiledID, ccp(buy_land_x, buy_land_y));
					refreshMoneyLabel(player2, -money);
					UFunction::playEffectMusicRandom(player2EffectVec_9, false);
				}
				break;
			}
			case MSG_BUY_LAND_2_TAG:
			{
				money = LAND_LEVEL_2_MONEY;
				if (playerTag == PLAYER_1_TAG)
				{
					_land->setTileGID(player1_building_3_tiledID, ccp(buy_land_x, buy_land_y));
					refreshMoneyLabel(player1, -money);
					UFunction::playEffectMusicRandom(player1EffectVec_9, false);
				}
				else
				{
					_land->setTileGID(player2_building_3_tiledID, ccp(buy_land_x, buy_land_y));
					refreshMoneyLabel(player2, -money);
					UFunction::playEffectMusicRandom(player2EffectVec_9, false);
				}
			}
			}
		}
		popupDialoge->setVisible(false);
		this->scheduleOnce(schedule_selector(GameMapBaseScene::InfoLotteryPublish), 1.0f);
		break;
	}
	case MSG_PAY_TOLLS_1_TAG://过路费
	{
		buy_land_x = atof(messageVector[1].c_str());
		buy_land_y = atof(messageVector[2].c_str());
		int playerTag = atoi(messageVector[3].c_str());
		if (itemTag == 7)
		{
			int endId = _land->getTileGIDAt(ccp(buy_land_x, buy_land_y));
			_land->setTileGID(blank_land_tiledID, ccp(buy_land_x, buy_land_y));
			itemTag = 0;
			if (endId == player1_building_2_tiledID)
				UFunction::playEffectMusicRandom(player1EffectVec_5, false);
			if (endId == player2_building_2_tiledID)
				UFunction::playEffectMusicRandom(player2EffectVec_5, false);
			NotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUY, String::createWithFormat("%d", MSG_BUY_LOTTERY_PUBLISH_TAG));
		}
		else if (itemTag==8)
		{
			itemTag = 0;
			if (playerTag == PLAYER_1_TAG)
				UFunction::playEffectMusicRandom(player1EffectVec_10, false);
			if (playerTag == PLAYER_2_TAG)
				UFunction::playEffectMusicRandom(player2EffectVec_10, false);
			NotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUY, String::createWithFormat("%d", MSG_BUY_LOTTERY_PUBLISH_TAG));
		}
		else
			payTolls(MSG_PAY_TOLLS_1_TAG, buy_land_x, buy_land_y, playerTag);
		break;
	}
	case MSG_PAY_TOLLS_2_TAG://过路费
	{
		buy_land_x = atof(messageVector[1].c_str());
		buy_land_y = atof(messageVector[2].c_str());
		int playerTag = atoi(messageVector[3].c_str());
		if (itemTag == 7)
		{
			int endId = _land->getTileGIDAt(ccp(buy_land_x, buy_land_y));
			_land->setTileGID(blank_land_tiledID, ccp(buy_land_x, buy_land_y));
			itemTag = 0;
			if (endId == player1_building_2_tiledID)
				UFunction::playEffectMusicRandom(player1EffectVec_5, false);
			if (endId == player2_building_2_tiledID)
				UFunction::playEffectMusicRandom(player2EffectVec_5, false);
			NotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUY, String::createWithFormat("%d", MSG_BUY_LOTTERY_PUBLISH_TAG));
		}
		else if (itemTag==8)
		{ 
			itemTag = 0;
			if (playerTag == PLAYER_1_TAG)
				UFunction::playEffectMusicRandom(player1EffectVec_10, false);
			if (playerTag == PLAYER_2_TAG)
				UFunction::playEffectMusicRandom(player2EffectVec_10, false);
			NotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUY, String::createWithFormat("%d", MSG_BUY_LOTTERY_PUBLISH_TAG));
		}
		else
			payTolls(MSG_PAY_TOLLS_2_TAG, buy_land_x, buy_land_y, playerTag);
		break;
	}
	case MSG_PAY_TOLLS_3_TAG://过路费
	{
		buy_land_x = atof(messageVector[1].c_str());
		buy_land_y = atof(messageVector[2].c_str());
		int playerTag = atoi(messageVector[3].c_str());
		if (itemTag == 7)
		{
			int endId = _land->getTileGIDAt(ccp(buy_land_x, buy_land_y));
			_land->setTileGID(blank_land_tiledID, ccp(buy_land_x, buy_land_y));
			itemTag = 0;
			if (endId == player1_building_2_tiledID)
				UFunction::playEffectMusicRandom(player1EffectVec_5, false);
			if (endId == player2_building_2_tiledID)
				UFunction::playEffectMusicRandom(player2EffectVec_5, false);
			NotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUY, String::createWithFormat("%d", MSG_BUY_LOTTERY_PUBLISH_TAG));
		}
		else if (itemTag == 8)
		{
			itemTag = 0;
			if (playerTag == PLAYER_1_TAG)
				UFunction::playEffectMusicRandom(player1EffectVec_10, false);
			if (playerTag == PLAYER_2_TAG)
				UFunction::playEffectMusicRandom(player2EffectVec_10, false);
			NotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUY, String::createWithFormat("%d", MSG_BUY_LOTTERY_PUBLISH_TAG));
		}
		else
			payTolls(MSG_PAY_TOLLS_3_TAG, buy_land_x, buy_land_y, playerTag);
		break;
	}
	case MSG_RANDOM_ASK_EVENT_TAG://随机事件
	{
		if (isTurn)
		{
			int playerTag = atoi(messageVector[3].c_str());
			move_or_end = atoi(messageVector[4].c_str());
			int randomNumber = rand() % (randomAskEventMap.size()) + TAX_REBATE_TAG;

			char temp[MAX_LEN];
			memset(temp, 0, MAX_LEN);
			sprintf_s(temp, "RandomAskEvent-%d-%d-%d", playerTag, move_or_end, randomNumber);
			cSocket.Send(temp, MAX_LEN);
		}
		break;
	}
	case MSG_RANDOM_DO_ASK_EVENT_TAG://处理随机事件
	{
		int playerTag = atoi(messageVector[1].c_str());
		move_or_end = atoi(messageVector[2].c_str());
		int randomNumber = atoi(messageVector[3].c_str());
		switch (playerTag){
		case PLAYER_1_TAG:
		{
			doRandomAskEvent(player1,randomNumber);
			break;
		}
		case PLAYER_2_TAG:
		{
			doRandomAskEvent(player2, randomNumber);
			break;
		}
		}
	}
	case MSG_GO_SHOW_MESSAGE_TAG://显示按钮
	{
		char temp[MAX_LEN];
		memset(temp, 0, MAX_LEN);
		sprintf_s(temp, "IsTurn-1");
		cSocket.Send(temp,MAX_LEN);
		break;
	}
	case MSG_GO_SHOW_TAG:
	{
		if (isTurn)
			refreshStrengthLabel(currentPlayer, -EACH_STRENGTH);
		isTurn = !isTurn;
		
		if (isTurn)
		{
			for (auto it = vecMenuItem.begin(); it != vecMenuItem.end(); it++)
			{
				Node* node = dynamic_cast<Node*>(*it);
				MoveBy* moveBy = MoveBy::create(0.3, ccp(-(node->getContentSize().width) * 2, 0));
				node->runAction(moveBy);

			}
			dice->getDice()->resume();
		}
		if (isTurn != isFirst)
		{
			gameRoundCount++;
			isPublish = false;
			refreshRoundDisplay();
		}
		break;
	}
	case  MSG_GO_HIDE_TAG://隐藏按钮
	{
		for (auto it = vecMenuItem.begin(); it != vecMenuItem.end(); it++)
		{
			Node* node = dynamic_cast<Node*>(*it);
			MoveBy* moveBy = MoveBy::create(0.3, ccp((node->getContentSize().width) * 2, 0));
			node->runAction(moveBy);

		}

		char temp[20];
		memset(temp, 0, 20);
		sprintf(temp, "dice_%02d.png", stepCount);
		dice->getDice()->setSpriteFrame(dice->getDiceFrameCache()->getSpriteFrameByName(temp));
		dice->getDice()->pause();
		break;
	}
	case MSG_GAME_OVER_TAG://游戏结束
	{
		NotificationCenter::getInstance()->removeAllObservers(this);
		int value = atoi(messageVector[1].c_str());
		switch (value){
		case 0:{
			std::string str = "your enemy has drop out of the line,so YOU WIN!!!";
			CocosToast::createToast(this, str.c_str(), 8 * ONE_SECOND, player_me->getPosition());
			this->scheduleOnce(schedule_selector(GameMapBaseScene::InfoGameOver), 8 * ONE_SECOND);
			break;
		}
		case 1:{
			int tag = atoi(messageVector[2].c_str());
			if (player_me->getTag() == tag)
			{
				std::string str = "YOUR LOSE!";
				CocosToast::createToast(this, str.c_str(), 8 * ONE_SECOND, Point(winSize/2));
			}
			else
			{
				std::string str = "YOUR WIN!";
				CocosToast::createToast(this, str.c_str(), 8 * ONE_SECOND, Point(winSize / 2));
			}
			this->scheduleOnce(schedule_selector(GameMapBaseScene::InfoGameOver), 8 * ONE_SECOND);
			break;
		}
		case 2:{
			int tag = atoi(messageVector[2].c_str());
			if (player_me->getStrength() * 100 + player_me->getMoney() > player_enemy->getStrength() * 100 + player_enemy->getMoney())
			{
				std::string str = "YOUR WIN!";
				CocosToast::createToast(this, str.c_str(), 8 * ONE_SECOND, Point(winSize / 2));
			}
			else if (player_me->getStrength() * 100 + player_me->getMoney() < player_enemy->getStrength() * 100 + player_enemy->getMoney())
			{
				std::string str = "YOUR LOSE!";
				CocosToast::createToast(this, str.c_str(), 8 * ONE_SECOND, Point(winSize / 2));
			}
			else
			{
				if (player_me->getTag() == tag)
				{
					std::string str = "YOUR LOSE!";
					CocosToast::createToast(this, str.c_str(), 8 * ONE_SECOND, Point(winSize / 2));
				}
				else
				{
					std::string str = "YOUR WIN!";
					CocosToast::createToast(this, str.c_str(), 8 * ONE_SECOND, Point(winSize / 2));
				}
			}
			this->scheduleOnce(schedule_selector(GameMapBaseScene::InfoGameOver), 8 * ONE_SECOND);
			break;
		}
		}
		break;
	}
	}
}
//----------------------------------------------------------------
void GameMapBaseScene::endCarGoAgain()	//救护车停下来
{
	currentPlayer->setVisible(false);
	int distance = emergencyCar->getPositionX();
	MoveBy* moveBy = MoveBy::create(1.0f, ccp(-distance, 0));
	Repeat* repeate = Repeat::create(emergencyCar->getCar_go_anmi(), 5);
	Sequence* spawnAction = Sequence::create(Spawn::create(moveBy, repeate, NULL), CallFunc::create(CC_CALLBACK_0(GameMapBaseScene::endCarGoLast, this)), NULL);
	spawnAction->retain();
	emergencyCar->runAction(spawnAction);
}
void GameMapBaseScene::endCarGoLast()
{
	emergencyCar->setVisible(false);
	CocosToast::createToast(this, String::createWithFormat("Need Rest %d Day", TRAP_DAY)->getCString(),4*ONE_SECOND,currentPlayer->getPosition());
	NotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUY, String::createWithFormat("%d", MSG_BUY_LOTTERY_PUBLISH_TAG));
}
void GameMapBaseScene::popupLotteryPublish(float dt)   //
{
	if (move_or_end == MSG_GO_PASS_TAG)
	{
		String * str = String::createWithFormat("%d", MSG_CONTROLLER_PASS_TAG);
		NotificationCenter::getInstance()->postNotification(MSG_CONTROLLER, str);
	}
	else
	{
		String * str = String::createWithFormat("%d", MSG_CONTROLLER_END_TAG);
		NotificationCenter::getInstance()->postNotification(MSG_CONTROLLER, str);
	}
}
void GameMapBaseScene::doRandomAskEvent(Player* player,int randomNumber)  //處理隨機事件
{
	
	std::string str = randomAskEventMap.at(randomNumber)->_string;

	switch (randomNumber)
	{
	case TAX_REBATE_TAG:
	{
		if (player->getTag() == PLAYER_1_TAG)
			UFunction::playEffectMusic(P1_Speaking_01052, false);
		else
			UFunction::playEffectMusic(P2_HENGCAI, false);
		refreshMoneyLabel(player, 5000);
		break;
	}
	case TAX_PAY_TAG:
	{
		if (player->getTag() == PLAYER_1_TAG)
			UFunction::playEffectMusic(P2_NASHUI, false);
		else
			UFunction::playEffectMusic(P1_Speaking_00452, false);
		refreshMoneyLabel(player, -5000);
		break;
	}

	case STRENGTH_LOSS_TAG:
	{
		UFunction::playEffectMusic(P1_tilihuisheng, false);
		refreshStrengthLabel(player, -40);
		break;
	}
	case STRENGTH_GET_TAG:
	{
		UFunction::playEffectMusic(P1_tilihuisheng, false);
		refreshStrengthLabel(player, 60);
		break;
	}
	case INVESTMENT_PROFIT_TAG:
	{
		UFunction::playEffectMusic(P1_Speaking_00181, false);
		refreshMoneyLabel(player, 10000);
		break;
	}
	case INVESTMENT_LOSS_TAG:
	{
		UFunction::playEffectMusic(P1_Speaking_00182, false);
		refreshMoneyLabel(player, -10000);
		break;
	}

	}
	CocosToast::createToast(this, str.c_str(), 4 * ONE_SECOND, player->getPosition());
	this->scheduleOnce(schedule_selector(GameMapBaseScene::Info), 4 * ONE_SECOND);
}
void  GameMapBaseScene::refreshStrengthLabel(Player* player, int strength)  //刷新體力
{
	int tag = player->getTag();
	int totalStrength = player->getStrength() + strength;
	
	player->setStrength(totalStrength);
	
		if (tag == PLAYER_1_TAG)
		{
			char money[MAX_LEN];
			memset(money, 0, MAX_LEN);
			sprintf_s(money, "Player1Strength-%d", player->getStrength());
			cSocket.Send(money, MAX_LEN);
		}
		if (tag == PLAYER_2_TAG)
		{
			char money[MAX_LEN];
			memset(money, 0, MAX_LEN);
			sprintf_s(money, "Player2Strength-%d", player->getStrength());
			cSocket.Send(money, MAX_LEN);
		}
}
void GameMapBaseScene::payTolls(int tollTag, float x, float y, int playerTag)  //付錢
{
	int money = 0;
	if (tollTag == MSG_PAY_TOLLS_1_TAG)
	{
		money = LAND_BLANK_MONEY;
	}
	if (tollTag == MSG_PAY_TOLLS_2_TAG)
	{
		money = LAND_LEVEL_1_MONEY;
	}
	if (tollTag == MSG_PAY_TOLLS_3_TAG)
	{
		money = LAND_LEVEL_2_MONEY;
	}


	Player* landOwner = getPlayerByTiled(buy_land_x, buy_land_y);


	switch (playerTag)
	{
	case PLAYER_1_TAG:
	{
		UFunction::playEffectMusicRandom(player1EffectVec_1, false);
		int retMoney = displayArea(x, y, player1, player2_building_1_tiledID, player2_building_2_tiledID, player2_building_3_tiledID);
		refreshMoneyLabel(landOwner, money + retMoney);
		refreshMoneyLabel(player1, -(money + retMoney));
		CocosToast::createToast(this, String::createWithFormat("+%d", money + retMoney)->getCString(), 2 * ONE_SECOND, landOwner->getPosition());
		CocosToast::createToast(this, String::createWithFormat("-%d", money + retMoney)->getCString(), 2 * ONE_SECOND, player1->getPosition());
		break;
	}
	case PLAYER_2_TAG:
	{
		UFunction::playEffectMusicRandom(player2EffectVec_1, false);
		int retMoney = displayArea(x, y, player2, player1_building_1_tiledID, player1_building_2_tiledID, player1_building_3_tiledID);
		refreshMoneyLabel(landOwner, money + retMoney);
		refreshMoneyLabel(player2, -(money + retMoney));
		CocosToast::createToast(this, String::createWithFormat("+%d", money + retMoney)->getCString(), 2 * ONE_SECOND, landOwner->getPosition());
		CocosToast::createToast(this, String::createWithFormat("-%d", money + retMoney)->getCString(), 2 * ONE_SECOND, player2->getPosition());
		break;
	}
	}
	NotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUY, String::createWithFormat("%d", MSG_BUY_LOTTERY_PUBLISH_TAG));
}
void GameMapBaseScene::showBuyLandDialoge(int landTag, int playerTag)  //買土地
{
	String showMessage = "";
	switch (landTag){
	case MSG_BUY_BLANK_TAG:
	{
		showMessage = String::createWithFormat("Do you want to buy the land ? need $ %d", LAND_BLANK_MONEY)->getCString();
		break;
	}
	case MSG_BUY_LAND_1_TAG:
	{
		showMessage = String::createWithFormat("Do you want to buy the land ? need $ %d", LAND_LEVEL_1_MONEY)->getCString();
		break;
	}
	case MSG_BUY_LAND_2_TAG:
	{
		showMessage = String::createWithFormat("Do you want to buy the land ? need $ %d", LAND_LEVEL_2_MONEY)->getCString();
		break;
	}
	}
	popupDialoge->getLabelContentText()->setString(showMessage.getCString());
	popupDialoge->setVisible(true);
}
void GameMapBaseScene::buyLandCallback(Node* node)               //買土地的囘調函數
{
	if (node->getTag() == okTag)
	{
		if (currentPlayer == player_me)
		{
			char temp[MAX_LEN];
			memset(temp, 0, MAX_LEN);
			sprintf_s(temp, "PlayerBuyLand-1");
			cSocket.Send(temp, MAX_LEN);
		}
		else
		{
			char temp[MAX_LEN];
			memset(temp, 0, MAX_LEN);
			sprintf_s(temp, "EnemyBuyLand-1");
			cSocket.Send(temp, MAX_LEN);
		}
	}
	else
	{
		if (currentPlayer == player_me)
		{
			char temp[MAX_LEN];
			memset(temp, 0, MAX_LEN);
			sprintf_s(temp, "PlayerBuyLand-0");
			cSocket.Send(temp, MAX_LEN);
		}
		else
		{
			char temp[MAX_LEN];
			memset(temp, 0, MAX_LEN);
			sprintf_s(temp, "EnemyBuyLand-0");
			cSocket.Send(temp, MAX_LEN);
		}
	}
}
void GameMapBaseScene::refreshMoneyLabel(Player* player, int money) //刷新金錢
{
	int tag = player->getTag();
	player->setMoney(player->getMoney() + money);
	
		if (tag == PLAYER_1_TAG)
		{
			char money[MAX_LEN];
			memset(money, 0, MAX_LEN);
			sprintf_s(money, "Player1Money-%d", player->getMoney());
			cSocket.Send(money, MAX_LEN);
		}
		if (tag == PLAYER_2_TAG)
		{
			char money[MAX_LEN];
			memset(money, 0, MAX_LEN);
			sprintf_s(money, "Player2Money-%d", player->getMoney());
			cSocket.Send(money, MAX_LEN);
		}
}
void GameMapBaseScene::refreshStock(Player* player, int number, int price)
{
	int tag = player->getTag();
	
	for (int i = 0; i < player->stockMap.size(); i++)
	{
		player->stockMap.at(i)->setMakedealprice(player->stockMap.at(i)->getMakedealprice() + price);
		player->stockMap.at(i)->setStoreNumber(player->stockMap.at(i)->getStoreNumber() + number);
		if (tag == PLAYER_1_TAG)
		{
			char money[MAX_LEN];
			memset(money, 0, MAX_LEN);
			sprintf_s(money, "Player1Stock-%d-%d-%d", i, player->stockMap.at(i)->getMakedealprice(), player->stockMap.at(i)->getStoreNumber());
			cSocket.Send(money, MAX_LEN);
		}
		if (tag == PLAYER_2_TAG)
		{
			char money[MAX_LEN];
			memset(money, 0, MAX_LEN);
			sprintf_s(money, "Player2Stock-%d-%d-%d", i, player->stockMap.at(i)->getMakedealprice(), player->stockMap.at(i)->getStoreNumber());
			cSocket.Send(money, MAX_LEN);
		}
	}
}
void GameMapBaseScene::updateStockVec()               //刷新股市
{
	float valule1 = rand() % 400 + 410;
	float valule2 = rand() % 400 + 410;
	float valule3 = rand() % 400 + 410;
	float valule4 = rand() % 400 + 410;
	float valule5 = rand() % 400 + 410;

	if (stock_pointvec1.size()>13)
	{
		for (int i = 0; i<13; i++)
		{
			stock_pointvec1.at(i) = stock_pointvec1.at(i + 1);
			stock_pointvec2.at(i) = stock_pointvec2.at(i + 1);
			stock_pointvec3.at(i) = stock_pointvec3.at(i + 1);
			stock_pointvec4.at(i) = stock_pointvec4.at(i + 1);
			stock_pointvec5.at(i) = stock_pointvec5.at(i + 1);

		}
		stock_pointvec1.at(13) = valule1;
		stock_pointvec2.at(13) = valule2;
		stock_pointvec3.at(13) = valule3;
		stock_pointvec4.at(13) = valule4;
		stock_pointvec5.at(13) = valule5;
	}
	else
	{
		stock_pointvec1.push_back(valule1);
		stock_pointvec2.push_back(valule2);
		stock_pointvec3.push_back(valule3);
		stock_pointvec4.push_back(valule4);
		stock_pointvec5.push_back(valule5);
	}
}

//----------------------------------------------------------------
//通知類函數
void GameMapBaseScene::InfoGameOver(float dt)
{
	cSocket.Close();
	Director::getInstance()->end();
}
void GameMapBaseScene::InfoEnd(float dt)
{
	String * str = String::createWithFormat("%d", MSG_CONTROLLER_AROUND_TAG);
	NotificationCenter::getInstance()->postNotification(MSG_CONTROLLER, str);
}
void GameMapBaseScene::InfoController(float dt)
{
	NotificationCenter::sharedNotificationCenter()->postNotification(MSG_CONTROLLER, String::createWithFormat("%d", MSG_CONTROLLER_END_TAG));
}

void GameMapBaseScene::InfoLotteryPublish(float dt)
{
	NotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUY, String::createWithFormat("%d", MSG_BUY_LOTTERY_PUBLISH_TAG));
}

void GameMapBaseScene::Info(float dt)
{
	if (move_or_end == MSG_GO_PASS_TAG)
	{
		String * str = String::createWithFormat("%d", MSG_CONTROLLER_PASS_TAG);
		NotificationCenter::getInstance()->postNotification(MSG_CONTROLLER, str);
	}
	else
	{
		String * str = String::createWithFormat("%d", MSG_CONTROLLER_AROUND_TAG);
		NotificationCenter::getInstance()->postNotification(MSG_CONTROLLER, str);
	}
}
//幫助類函數
Player* GameMapBaseScene::getPlayerByTiled(float x, float y)   //獲取當前角色
{
	int gid = _land->getTileGIDAt(ccp(x,y));
	if(gid == player1_building_1_tiledID || gid == player1_building_2_tiledID || gid == player1_building_3_tiledID)
	{
			return player1;
	}
	if(gid == player2_building_1_tiledID || gid == player2_building_2_tiledID || gid == player2_building_3_tiledID)
	{
			return player2;
	}
}
//相連土地累計付錢
int GameMapBaseScene::displayArea(float x, float y, Player* player, int building_1_tiledID, int building_2_tiledID, int building_3_tiledID)
{
	int sumMoney = 0;
	float retX = UFunction::GL2map(player->getPosition(), _map).x;
	if (x == retX)
	{
		float leftX = x - 1;
		float rightX = x + 1;
		int leftGID = _land->getTileGIDAt(ccp(leftX, y));
		int rightGID = _land->getTileGIDAt(ccp(rightX, y));
		
		while (leftGID != 0 && (leftGID == building_1_tiledID || leftGID == building_2_tiledID || leftGID == building_3_tiledID))
		{
			if (leftGID == building_1_tiledID)
			{
				sumMoney += LAND_BLANK_MONEY;
			}
			if (leftGID == building_2_tiledID)
			{
				sumMoney += LAND_LEVEL_1_MONEY;
			}
			if (leftGID == building_3_tiledID)
			{
				sumMoney += LAND_LEVEL_2_MONEY;
			}
			leftX -= 1;
			leftGID = _land->getTileGIDAt(ccp(leftX, y));

			if (leftGID == 0)
			{
				break;
			}
		}
		while (rightGID != 0 && (rightGID == building_1_tiledID || rightGID == building_2_tiledID || rightGID == building_3_tiledID))
		{
			if (rightGID == building_1_tiledID)
			{
				sumMoney += LAND_BLANK_MONEY;
			}
			if (rightGID == building_2_tiledID)
			{
				sumMoney += LAND_LEVEL_1_MONEY;
			}
			if (rightGID == building_3_tiledID)
			{
				sumMoney += LAND_LEVEL_2_MONEY;
			}
		
			rightX += 1;
			rightGID = _land->getTileGIDAt(ccp(rightX, y));

			if (rightGID == 0)
			{
				break;
			}
		}
	}

	float retY = UFunction::GL2map(player->getPosition(), _map).y;
	if (y == retY)
	{
		float upY = y - 1;
		float downY = y + 1;
		int upGID = _land->getTileGIDAt(ccp(x, upY));
		int downGID = _land->getTileGIDAt(ccp(x, downY));
		
		while (upGID != 0 && (upGID == building_1_tiledID || upGID == building_2_tiledID || upGID == building_3_tiledID))
		{
			if (upGID == building_1_tiledID)
			{
				sumMoney += LAND_BLANK_MONEY;
			}
			if (upGID == building_2_tiledID)
			{
				sumMoney += LAND_LEVEL_1_MONEY;
			}
			if (upGID == building_3_tiledID)
			{
				sumMoney += LAND_LEVEL_2_MONEY;
			}
			
			upY -= 1;
			upGID = _land->getTileGIDAt(ccp(x, upY));

			if (upGID == 0)
			{
				break;
			}
		}
		while (downGID != 0 && (downGID == building_1_tiledID || downGID == building_2_tiledID || downGID == building_3_tiledID))
		{
			if (downGID == building_1_tiledID)
			{
				sumMoney += LAND_BLANK_MONEY;
			}
			if (downGID == building_2_tiledID)
			{
				sumMoney += LAND_LEVEL_1_MONEY;
			}
			if (downGID == building_3_tiledID)
			{
				sumMoney += LAND_LEVEL_2_MONEY;
			}
		
			downY += 1;
			downGID = _land->getTileGIDAt(ccp(x, downY));

			if (downGID == 0)
			{
				break;
			}
		}
	}
	return sumMoney;
}
//-----------------------------------------------------------------------
bool GameMapBaseScene::connectServer()  //連接服務器
{
	std::thread receiveThread = std::thread(&GameMapBaseScene::receiveData, this);
	receiveThread.detach();

	if (isTurn == 1)
	{
		currentPlayer = player1;
		isTurn = true; isFirst = true;
		player_me = player1;
		player_enemy = player2;
		isStart = true;
	}
	if (isTurn == 0)
	{
		currentPlayer = player2;
		isTurn = false; isFirst = false;
		player_me = player2;
		player_enemy = player1;
		isStart = true;

		Vector<Node*> vecMenuItem = menu->getChildren();
		for (auto it = vecMenuItem.begin(); it != vecMenuItem.end(); it++)
		{
			Node* node = dynamic_cast<Node*>(*it);
			MoveBy* moveBy = MoveBy::create(0.3, ccp((node->getContentSize().width) * 2, 0));
			node->runAction(moveBy);

		}
	}
	char temp[MAX_LEN];
	memset(temp, 0, MAX_LEN);
	sprintf_s(temp, "isStart-1");
	cSocket.Send(temp, MAX_LEN);

	return true;
}
void GameMapBaseScene::receiveData() //接收數據
{
	while (true)
	{
		char receiveBuf[MAX_LEN] = "";
		int iResult = cSocket.Recv(receiveBuf, MAX_LEN, 0);
		if (iResult <= 0)
			break;

		EnterCriticalSection(&g_cs);
			std::vector<std::string>temp;
			temp.clear();
			UFunction::splitString(receiveBuf, " ", temp);
			for (int i = 0; i < temp.size(); i++)
			{
				std::string str = temp[i];
				SocketDataManager::getInstance()->pushMsg(str);
			}
		LeaveCriticalSection(&g_cs);

	}
}

