#ifndef __START_GAME_SCENE_H__
#define __START_GAME_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;
//��ʼ����
class StartGameScene :public Layer{
public:
	static Scene* createScene(); //��̬��������������ʵ�����
	virtual bool init();
	CREATE_FUNC(StartGameScene);
	static void end();
	static void run();
};

#endif