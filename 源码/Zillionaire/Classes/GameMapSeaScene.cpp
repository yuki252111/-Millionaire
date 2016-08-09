#include "GameMapSeaScene.h"

Scene* GameMapSeaScene::createScene()	//���س���
{
	auto scene = Scene::create();
	auto layer = GameMapSeaScene::create();
	scene->addChild(layer);
	return scene;
}
bool GameMapSeaScene::init()	//��ʼ��
{
	if (!GameMapBaseScene::init())
		return false;
	return true;
}
void GameMapSeaScene::addRightBanner()
{
	Sprite* rightBanner = Sprite::create("SeaRightBanner.jpg");
	rightBanner->setAnchorPoint(ccp(0, 0));
	rightBanner->setPosition(ccp(tableStartPosition_x, 0));
	this->addChild(rightBanner);
}
void GameMapSeaScene::addMap()        //���ص�ͼ
{
	_map = TMXTiledMap::create("sea.tmx");
	this->addChild(_map);
}
void GameMapSeaScene::initTiledGrid()         //��ʼ����ͼ
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
void GameMapSeaScene::initPropTiledID()	//���ص���
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
