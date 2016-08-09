#ifndef __GAME_MAP_BASE_SCENE_H__
#define __GAME_MAP_BASE_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Player.h"
#include "RouteNavigation.h"
#include "UDefine.h"
#include "Dice.h"
#include "PopupLayer.h"
#include "UFunction.h"
#include "ODSocket.h"
#include "StrengthUp.h"
#include "LotteryPublish.h"
#include "SimpleAudioEngine.h"
#include "EmergencyCar.h"

USING_NS_CC;
USING_NS_CC_EXT;

const int tableStartPosition_x = 650;
const int tableStartPosition_y = 450;
const int tableWidth = 50;
const int tableHeight = 40;
const int tiledWidth = 32;
const int tiledHeight = 32;

const int fontSize = 20;
const int dialogeWidth = 400;
const int dialogeHeight = 220;

const int okTag = 1;
const int cancelTag = 0;
//游戏场景
class GameMapBaseScene : public Layer
{
public:
	static Scene* createScene();
	static void drawPathMark(std::vector<int> rowVector, std::vector<int> colVector);
	virtual bool init();
	virtual void addMap();
	void addLand();

	virtual void initTiledGrid();
	virtual void initPropTiledID();
	virtual void addRightBanner();
	
	CREATE_FUNC(GameMapBaseScene);

	int tiledColsCount;
	int tiledRowsCount;
	bool** canPassGrid;
	std::vector<Point> wayPassPosition;
	//related to map
	static TMXTiledMap* _map;
	static TMXLayer* _land;
	static TMXLayer* wayLayer;
	static Point shopPoint;
	static Point godPoint1;
	static Point godPoint2;
	static Point trapPoint;
	static Vector<Sprite*>pathMarkVector;
	//related to player
	static std::vector<int> colVector;
	static std::vector<int> rowVector;
	
	static CRITICAL_SECTION g_ms;

	static int blank_land_tiledID;
	static int strength_30_tiledID;
	static int strength_50_tiledID;
	static int strength_80_tiledID;

	static int randomEvent_tiledID;
	static int lottery_tiledID;
	static int stock_tiledID;
	static int shop_tiledID;

	static int player1_building_1_tiledID;
	static int player1_building_2_tiledID;
	static int player1_building_3_tiledID;

	static int player2_building_1_tiledID;
	static int player2_building_2_tiledID;
	static int player2_building_3_tiledID;


	static std::vector<float> stock_pointvec1;
	static std::vector<float> stock_pointvec2;
	static std::vector<float> stock_pointvec3;
	static std::vector<float> stock_pointvec4;
	static std::vector<float> stock_pointvec5;

	//-----------------------------------------------------
	static bool isTurn;
	static bool isStart;
	static int selectedMap;
	static ODSocket cSocket;

	~GameMapBaseScene();
private:

	Dice* dice;
	Menu* menu;
	Vector<SpriteFrame*>  digiteRoundVector;
	Vector<Sprite*> refreshRoundVector;
	int gameRoundCount;  //第几个回合
	int stepCount;  //色子的点数
	//地图相关
	StrengthUp* strengthUpInstance;
	Sprite* shop;
	Sprite* landGod1;
	Sprite* landGod2;
	Sprite* trap;
	EmergencyCar* emergencyCar;
	Player* player1;
	Player* player2;
	Player* player_me;
	Player* player_enemy;
	Player* currentPlayer;
	LabelTTF * player1_money_label;
	LabelTTF * player2_money_label;
	LabelTTF * player1_strength_label;
	LabelTTF * player2_strength_label;

	//弹出框相关
	PopupLayer* popupDialoge;
	PopupLayer* lotteryDialoge;
	PopupLayer* lotteryPublishDialoge;
	PopupLayer* shopItemDialoge;
	PopupLayer* packageDialoge;
	Map<int, String*> randomAskEventMap;
	int itemTag;
	
	float buy_land_x;
	float buy_land_y;
	int move_or_end;
	CRITICAL_SECTION g_cs;
	CRITICAL_SECTION msg;
	//音效相关
	std::vector<char*>bgMusicVector;

	Vector<String*> player2EffectVec_1;//交过路费声音0-6
	Vector<String*> player2EffectVec_2;//抢夺别人地块7-10
	Vector<String*> player2EffectVec_3;//房屋被抢夺11-14
	Vector<String*> player2EffectVec_4;//摧毁别人房屋15-18
	Vector<String*> player2EffectVec_5;//房屋被摧毁19-22
	Vector<String*> player2EffectVec_6;//

	Vector<String*> player2EffectVec_7;//看到别人住院27-28
	Vector<String*> player2EffectVec_8; // 收取过路费29 - 34
	Vector<String*> player2EffectVec_9;//升级房子35-36
	Vector<String*> player2EffectVec_10; // 不交过路费37 - 38
	Vector<String*> player2EffectVec_11; // 买地39 - 42
	Vector<String*> player2EffectVec_12;//
	Vector<String*> player2EffectVec_13;//对方被罚收税45-48


	Vector<String*> player1EffectVec_1;//交过路费声音0-6
	Vector<String*> player1EffectVec_2;//抢夺别人地块7-10
	Vector<String*> player1EffectVec_3;//房屋被抢夺11-14
	Vector<String*> player1EffectVec_4;//摧毁别人房屋15-18
	Vector<String*> player1EffectVec_5;//房屋被摧毁19-22
	Vector<String*> player1EffectVec_6;//

	Vector<String*> player1EffectVec_7;//看到别人住院27-28
	Vector<String*> player1EffectVec_8; // 收取过路费29 - 34
	Vector<String*> player1EffectVec_9;//升级房子35-36
	Vector<String*> player1EffectVec_10; // 不交过路费37 - 38
	Vector<String*> player1EffectVec_11; // 买地39 - 42
	Vector<String*> player1EffectVec_12;//
	Vector<String*> player1EffectVec_13;//对方被罚收税45-48

	void setWayPassGrid();
	//地图相关
	
	void drawTable(int playerNumber);
	void addGoButton();
	void goButtonCallFunc(CCObject* pSender);
	void addDice();
	void addDigiteRoundSprite();

	void initItem();
	void addPathMark();
	void addPlayer();
	void addPlayerInfo();

	void refreshRoundDisplay();
	//弹出框相关
	void initPopupDialoge();
	void initLotteryDialoge();
	void initLotteryPublishDialoge();
	void initShopItemDialoge();
	void initPackageDialoge();
	void initRandomAskEvent();
	void initAudioEffect();
	//回调函数
	void buyLotteryCallback(Node* node);
	void buyShopItemCallback(Node* node);
	void packageCallback(Node*);
	void doRandomAskEvent(Player* player,int randnum);
	void buyLandCallback(Node *node);
	void playerBgMusic(float dt);
	void endCarGoAgain();
	void endCarGoLast();
	
	void registerObserver();
	void dealMsg(Object* data);
	//更新相关
	void showBuyLandDialoge(int,int);
	void payTolls(int payTag, float x, float y, int playerTag);
	void refreshMoneyLabel(Player* player, int money);
	void refreshStrengthLabel(Player* player, int strength);
	void refreshStock(Player* player, int number, int price);
	void popupLotteryPublish(float);
	void updateStockVec();
	//通知相关
	void Info(float);
	void InfoEnd(float dt);
	void InfoLotteryPublish(float);
	void InfoController(float dt);
	void InfoGameOver(float dt);
	Player* getPlayerByTiled(float x, float y);
	int displayArea(float x, float y, Player* player, int building_1_tiledID, int building_2_tiledID, int building_3_tiledID);

	//---------------------------------------------------------------------
	bool isFirst;
	bool isPublish;

	bool connectServer();
	void receiveData();
};

#endif