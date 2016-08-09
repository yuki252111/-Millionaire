#ifndef __GAME_MAP_CHOICE_SCENE_H__
#define __GAME_MAP_CHOICE_SCENE_H__

#include "cocos2d.h"
#include "editor-support\cocostudio\CCSGUIReader.h"
#include "ui\CocosGUI.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocostudio;
//选择地图
class GameMapChoiceScene :public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameMapChoiceScene);
private:
	int currentMap;
	Sprite* beachMap;
	Sprite* seaMap;
	Sprite* snowMap;
	Menu* menu;
	Sprite* bgSprite;

	void onclickBack(CCObject* pSender); //菜单回调函数
	void onclickForward(CCObject* pSender);
	void changeMap(float dt);            //自动改变场景
};

#endif