#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Player.h"
#include "UFunction.h"

USING_NS_CC;
USING_NS_CC_EXT;

const float playerGoTotalTime = 0.28f;
const float playerGoPerFrameTime = 0.07f;
//控制游戏，单例类
class GameController :public Layer
{
public:
	static GameController* gameControllerInstance;
	static GameController* getInstance();	//创建单例
	~GameController();
	void startGo(std::vector<int> rowVector, std::vector<int> colVector, Player* player);//控制游戏角色行走
	void endGo();
	CREATE_FUNC(GameController);
	virtual bool init();
private:
	int stepHasGone;	//已走	
	int stepsCount;		//总共要走
	int nextRow;		//下一行
	int nextCol;		//下一列
	int currentRow;		//当前行
	int currentCol;		//当前列
	MoveBy* moveBy;
	Repeat* repeate;
	Action* spawnAction;
	CallFunc* callEndGoFunc;
	Player* player;
	float** positionAroundEnd;
	
	std::vector<int> rowVector;	//行向量
	std::vector<int> colVector;	//列向量

	void moveOneStep(Player*);	//下一步
	void handlePropEvent();             //处理道具
	void aroundLandEvent();             //处理土地
	void registerObserver();            //注册消息
	void dealMsg(Object* data);        //处理消息
};

#endif