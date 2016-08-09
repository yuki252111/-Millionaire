#include "GameMapChoiceScene.h"
#include "UDefine.h"
#include "GameMapSeaScene.h"
#include "GameMapBeachScene.h"
#include "GameMapSnowScene.h"
#include "GameHall.h"

Scene* GameMapChoiceScene::createScene()	//���س���
{
	auto scene = Scene::create();
	auto layer = GameMapChoiceScene::create();
	scene->addChild(layer);
	return scene;
}
bool GameMapChoiceScene::init()  //��ʼ��
{
	if (!Layer::init())
		return false;
	currentMap = 0;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	bgSprite = Sprite::create("stage_background.png");   //���ر���ͼ
	bgSprite->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bgSprite, 0);

	//���ز˵��Լ��˵���
	menu = Menu::create();
	menu->setPosition(CCPointZero);

	MenuItemImage* menuImage = MenuItemImage::create("stage_button2.png", "stage_button1.png", this, menu_selector(GameMapChoiceScene::onclickBack));
	Size imenu = menuImage->getContentSize();
	menuImage->setPosition(ccp(imenu.width / 2, imenu.height / 2));
	LabelTTF* ttf = LabelTTF::create("<< BACK", "", 20);
	ttf->setColor(ccc3(0, 0, 0));
	ttf->setPosition(ccp(imenu.width / 2, imenu.height / 2));
	menuImage->addChild(ttf);
	menu->addChild(menuImage);

	MenuItemImage* menuImage2 = MenuItemImage::create("stage_button2.png", "stage_button1.png", this, menu_selector(GameMapChoiceScene::onclickForward));
	Size imenu2 = menuImage2->getContentSize();
	menuImage2->setPosition(ccp(visibleSize.width-imenu2.width / 2, imenu2.height / 2));
	LabelTTF* ttf2 = LabelTTF::create("FORWARD >>", "", 20);
	ttf2->setColor(ccc3(0, 0, 0));
	ttf2->setPosition(ccp(imenu2.width / 2, imenu2.height / 2));
	menuImage2->addChild(ttf2);
	menu->addChild(menuImage2);

	this->addChild(menu);
	//���ص�ͼ
	beachMap = Sprite::create("BeachMap.png");
	seaMap = Sprite::create("SeaMap.png");
	snowMap = Sprite::create("SnowMap.png");
	beachMap->setPosition(Point(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	seaMap->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	snowMap->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	seaMap->setVisible(false);
	snowMap->setVisible(false);

	this->addChild(beachMap, 1);
	this->addChild(seaMap, 1);
	this->addChild(snowMap, 1);
	this->schedule(schedule_selector(GameMapChoiceScene::changeMap),ONE_SECOND*2);
	return true;
}
void GameMapChoiceScene::onclickBack(CCObject* pSender)  //����
{
	Director::getInstance()->popScene();	
}
void GameMapChoiceScene::onclickForward(CCObject* pSender) //������Ϸ����
{
	switch (currentMap){
	case 0:
		GameMapBaseScene::selectedMap = 0;
		Director::getInstance()->pushScene(TransitionFadeBL::create(2 * ONE_SECOND, GameHall::createScene()));
		break;
	case 1:
		GameMapBaseScene::selectedMap = 1;
		Director::getInstance()->pushScene(TransitionFadeBL::create(2 * ONE_SECOND, GameHall::createScene()));
		break;
	case 2:
		GameMapBaseScene::selectedMap = 2;
		Director::getInstance()->pushScene(TransitionFadeBL::create(2 * ONE_SECOND, GameHall::createScene()));
		break;
	}
}
void GameMapChoiceScene::changeMap(float dt)  //�л�����
{
	currentMap++;
	currentMap %= MAP_COUNT;
	switch (currentMap){
	case 0:                                    //ɳ̲
		beachMap->setVisible(true);
		seaMap->setVisible(false);
		snowMap->setVisible(false);
		break;
	case 1:                                    //����
		beachMap->setVisible(false);
		seaMap->setVisible(true);
		snowMap->setVisible(false);
		break;
	case 2:                                    //����ѩ��
		beachMap->setVisible(false);
		seaMap->setVisible(false);
		snowMap->setVisible(true);
		break;
	}

}