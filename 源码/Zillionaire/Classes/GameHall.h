#ifndef __GAME_HALL_H__
#define __GAME_HALL_H__

#include "cocos2d.h"
#include "editor-support\cocostudio\CCSGUIReader.h"
#include "ui\CocosGUI.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocostudio;
//游戏大厅
class GameHall :public Layer{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameHall);
private:
	Vector<Button*>posButtons;//位置按钮
	std::vector<Point>posPoints;	//位置点
	Vector<Sprite*>players;	//用户精灵
	int curpos;	//当前位置
	LabelTTF* sign;	//当前位置指示符

	std::vector<std::string>msg;	//消息
	CRITICAL_SECTION g_cs;
	CRITICAL_SECTION m_cs;

	void onclick(Ref*,TouchEventType type);	//回调函数
	bool connect();	//连接服务器
	void receiveData();	//接收数据
	virtual void update(float dt);	//处理数据
};

#endif