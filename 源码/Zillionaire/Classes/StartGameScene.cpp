#include "StartGameScene.h"
#include "cocos-ext.h"
#include "UDefine.h"
#include "GameMenuScene.h"

Scene* StartGameScene::createScene()	//加载场景
{
	auto scene = Scene::create();
	auto layer = StartGameScene::create();
	scene->addChild(layer);
	return scene;
}
bool StartGameScene::init()            //初始化函数
{
	if (!Layer::init())
		return false;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	
	
	cocos2d::extension::Scale9Sprite* sprite = cocos2d::extension::Scale9Sprite::create("StartGameScene.png");
	sprite->setPosition(visibleSize.width / 2+origin.x, visibleSize.height / 2+origin.y);
	sprite->setOpacity(0);
	this->addChild(sprite);

	FadeIn* fadeIn = FadeIn::create(ONE_SECOND * 2);
	FadeOut* fadeOut = FadeOut::create(ONE_SECOND * 2);
	
	auto callbackFunc = [](){
		end();
	};
	CallFunc* callFunc = CallFunc::create(callbackFunc);

	Action* actions = Sequence::create(fadeIn,fadeOut, callFunc, NULL);
	sprite->runAction(actions);
	return true;
}
void StartGameScene::run()        //加载场景
{
	Director::getInstance()->runWithScene(createScene());
}
void StartGameScene::end()         //结束场景
{
	auto  scene = TransitionFadeBL::create(2 * ONE_SECOND, GameMenuScene::createScene());
	Director::getInstance()->replaceScene(scene);
}