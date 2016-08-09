#include "GameMenuScene.h"
#include "UDefine.h"
#include "GameMapChoiceScene.h"
#include "GameMusicScene.h"
USING_NS_CC;
USING_NS_CC_EXT;

cocos2d::Scene* GameMenuScene::createScene()	//加载场景
{
	auto scene = Scene::create();
	auto layer = GameMenuScene::create();
	scene->addChild(layer);
	return scene;
}
bool GameMenuScene::init()  //初始化
{
	if (!Layer::init())
		return false;
	GameMusicScene::bgOn = true;
	GameMusicScene::effOn = true;

	Size winSize = CCDirector::getInstance()->getWinSize();

	menu = Menu::create();  //建立菜单
	menu->setPosition(CCPointZero);
	//建立菜单项
	MenuItemImage* menuImage = MenuItemImage::create(BUTTON_BG2, BUTTON_BG1, this, menu_selector(GameMenuScene::onclickStart));
	menuImage->setPosition(ccp(winSize.width/2,winSize.height/4*3));
	Size imenu = menuImage->getContentSize();
	LabelTTF* ttf = LabelTTF::create("Start", "", 20);
	ttf->setColor(ccc3(0, 0, 0));
	ttf->setPosition(ccp(imenu.width / 2, imenu.height / 2));
	menuImage->addChild(ttf);
	menu->addChild(menuImage);

	MenuItemImage* menuImage2 = MenuItemImage::create(BUTTON_BG2, BUTTON_BG1, this, menu_selector(GameMenuScene::onclickMusic));
	menuImage2->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	Size imenu2 = menuImage2->getContentSize();
	LabelTTF* ttf2 = LabelTTF::create("Music", "", 20);
	ttf2->setColor(ccc3(0, 0, 0));
	ttf2->setPosition(ccp(imenu2.width / 2, imenu2.height / 2));
	menuImage2->addChild(ttf2);
	menu->addChild(menuImage2);

	MenuItemImage* menuImage3 = MenuItemImage::create(BUTTON_BG2, BUTTON_BG1, this, menu_selector(GameMenuScene::onclickEnd));
	menuImage3->setPosition(ccp(winSize.width / 2, winSize.height / 4));
	Size imenu3 = menuImage3->getContentSize();
	LabelTTF* ttf3 = LabelTTF::create("End", "", 20);
	ttf3->setColor(ccc3(0, 0, 0));
	ttf3->setPosition(ccp(imenu3.width / 2, imenu3.height / 2));
	menuImage3->addChild(ttf3);
	menu->addChild(menuImage3);

	this->addChild(menu,1);
	//背景
	bgSprite = Sprite::create("GameMenuBG.jpg");
	bgSprite->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	this->addChild(bgSprite, 0);
	return true;
}

void GameMenuScene::onclickStart(CCObject* pSender)  //start回调函数
{
	Director::getInstance()->pushScene(TransitionFadeBL::create(2* ONE_SECOND, GameMapChoiceScene::createScene()));		
}
void GameMenuScene::onclickMusic(CCObject* pSender) //music 回调函数
{
	Director::getInstance()->pushScene(TransitionFadeBL::create(2 * ONE_SECOND, GameMusicScene::createScene()));
}
void GameMenuScene::onclickEnd(CCObject* pSender) //end 回调函数
{
	Director::getInstance()->end();
}