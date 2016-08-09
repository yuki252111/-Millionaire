#ifndef __START_GAME_SCENE_H__
#define __START_GAME_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;
//开始场景
class StartGameScene :public Layer{
public:
	static Scene* createScene(); //静态函数，调用生成实体对象
	virtual bool init();
	CREATE_FUNC(StartGameScene);
	static void end();
	static void run();
};

#endif