#include "GameMapSnowScene.h"

Scene* GameMapSnowScene::createScene()	//加载场景
{
	auto scene = Scene::create();
	auto layer = GameMapSnowScene::create();
	scene->addChild(layer);
	return scene;
}
bool GameMapSnowScene::init()	//初始化
{
	if (!GameMapBaseScene::init())
		return false;
	return true;
}
void GameMapSnowScene::addRightBanner()
{
	Sprite* rightBanner = Sprite::create("SnowRightBanner.jpg");
	rightBanner->setAnchorPoint(ccp(0, 0));
	rightBanner->setPosition(ccp(tableStartPosition_x, 0));
	this->addChild(rightBanner);
}
void GameMapSnowScene::addMap()            //初始化地图
{
	_map = TMXTiledMap::create("snow.tmx");
	this->addChild(_map);
}
void GameMapSnowScene::initTiledGrid()          //初始化地图
{
	tiledColsCount = 20;
	tiledRowsCount = 15;
	canPassGrid = new  bool*[tiledRowsCount];
	for (int i = 0; i<tiledRowsCount; i++)
		canPassGrid[i] = new bool[tiledColsCount];



	for (int row = 0; row<tiledRowsCount; row++)
	{
		for (int col = 0; col<tiledColsCount; col++)
		{
			canPassGrid[row][col] = false;
		}
	}
}
void GameMapSnowScene::initPropTiledID()	//加载道具
{
	blank_land_tiledID = 1;
	strength_30_tiledID = 4;
	strength_50_tiledID = 5;
	strength_80_tiledID = 6;

	randomEvent_tiledID = 7;
	lottery_tiledID = 8;
	stock_tiledID = 9;

	player1_building_1_tiledID = 10;
	player1_building_2_tiledID = 11;
	player1_building_3_tiledID = 12;

	player2_building_1_tiledID = 13;
	player2_building_2_tiledID = 14;
	player2_building_3_tiledID = 15;

}
