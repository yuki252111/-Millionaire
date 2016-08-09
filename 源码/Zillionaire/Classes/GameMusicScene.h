#ifndef __GAME_MUSIC_H__
#define __GAME_MUSIC_H__

#include "cocos2d.h"
#include "editor-support\cocostudio\CCSGUIReader.h"
#include "ui\CocosGUI.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocostudio;
//“Ù–ß
class GameMusicScene :public Layer{
public:
	static Scene* createScene();
	static Scene* musicInstance;
	static bool bgOn;
	static bool effOn;
	virtual bool init();
	CREATE_FUNC(GameMusicScene);
private:
	
	Sprite* bgMusicOn;
	Sprite* bgMusicOff;
	Sprite* effMusicOn;
	Sprite* effMusicOff;

	void bgOnclick(Ref* ref, TouchEventType type);
	void effOnclick(Ref* ref, TouchEventType type);
	void backOnclick(Ref* ref, TouchEventType type);
};

#endif