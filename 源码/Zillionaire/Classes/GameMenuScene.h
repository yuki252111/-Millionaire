#ifndef __GAME_MENU_SCENE_H__
#define __GAME_MENU_SCENE_H__

#include "cocos2d.h"
#include "editor-support\cocostudio\CCSGUIReader.h"
#include "ui\CocosGUI.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocostudio;
//菜单
class GameMenuScene :public Layer{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameMenuScene);
private:
	Menu* menu;
	Sprite* bgSprite;

	void onclickStart(CCObject* pSender);  //菜单回调函数
	void onclickMusic(CCObject* pSender);
	void onclickEnd(CCObject* pSender);
};

#endif