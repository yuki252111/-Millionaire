#include "GameMusicScene.h"
#include "UDefine.h"

Scene* GameMusicScene::musicInstance = NULL;
bool GameMusicScene::bgOn = true;
bool GameMusicScene::effOn = true;

Scene* GameMusicScene::createScene()	//加载场景
{
	if (musicInstance == NULL)
	{
		musicInstance = Scene::create();
		auto layer = GameMusicScene::create();
		musicInstance->addChild(layer);
		musicInstance->retain();
	}
	
	return musicInstance;
}
bool GameMusicScene::init()	//初始化
{
	if (!Layer::init())
		return false;
	auto UI = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("GameMusic.json");
	this->addChild(UI);
	UI->setPosition(Point(160, 80));
	bgOn = true;
	effOn = true; 
	//加载按钮
	Button* bgButton = (Button*)Helper::seekWidgetByName(UI, "BGMusic");
	Button* effButton = (Button*)Helper::seekWidgetByName(UI, "Effect");
	Button* backButton = (Button*)Helper::seekWidgetByName(UI, "Back");
	bgButton->addTouchEventListener(this, toucheventselector(GameMusicScene::bgOnclick));
	effButton->addTouchEventListener(this, toucheventselector(GameMusicScene::effOnclick));
	backButton->addTouchEventListener(this, toucheventselector(GameMusicScene::backOnclick));

	Point bgPoint = bgButton->getPosition()+Point(160,80);
	Point effPoint = effButton->getPosition() + Point(160, 80);

	bgMusicOn = Sprite::create("bg_on.png");
	bgMusicOff = Sprite::create("bg_off.png");
	bgMusicOn->setPosition(bgPoint);
	bgMusicOff->setPosition(bgPoint);

	effMusicOn = Sprite::create("eff_on.png");
	effMusicOff = Sprite::create("eff_off.png");
	effMusicOn->setPosition(effPoint);
	effMusicOff->setPosition(effPoint);

	this->addChild(bgMusicOn);
	this->addChild(bgMusicOff);
	this->addChild(effMusicOn);
	this->addChild(effMusicOff);

	bgMusicOn->setVisible(true);
	effMusicOn->setVisible(true);

	bgMusicOff->setVisible(false);
	effMusicOff->setVisible(false);
	return true;
}
void GameMusicScene::bgOnclick(Ref* ref, TouchEventType type)	//回调函数
{
	switch (type){
	case TouchEventType::TOUCH_EVENT_BEGAN:break;
	case TouchEventType::TOUCH_EVENT_MOVED:break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		bgOn = !bgOn;
		if (bgOn)
		{
			bgMusicOn->setVisible(true);
			bgMusicOff->setVisible(false);
		}
		else
		{
			bgMusicOn->setVisible(false);
			bgMusicOff->setVisible(true);
		}
		break;
	}
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	}
}
void GameMusicScene::effOnclick(Ref* ref, TouchEventType type)	//回调函数
{
	switch (type){
	case TouchEventType::TOUCH_EVENT_BEGAN:break;
	case TouchEventType::TOUCH_EVENT_MOVED:break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		effOn = !effOn;
		if (effOn)
		{
			effMusicOn->setVisible(true);
			effMusicOff->setVisible(false);
		}
		else
		{
			effMusicOn->setVisible(false);
			effMusicOff->setVisible(true);
		}
		break;
	}
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	}
}
void GameMusicScene::backOnclick(Ref* ref, TouchEventType type)	//回调函数
{
	Director::getInstance()->popScene();
}