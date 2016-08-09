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
//������Ϸ��������
class GameController :public Layer
{
public:
	static GameController* gameControllerInstance;
	static GameController* getInstance();	//��������
	~GameController();
	void startGo(std::vector<int> rowVector, std::vector<int> colVector, Player* player);//������Ϸ��ɫ����
	void endGo();
	CREATE_FUNC(GameController);
	virtual bool init();
private:
	int stepHasGone;	//����	
	int stepsCount;		//�ܹ�Ҫ��
	int nextRow;		//��һ��
	int nextCol;		//��һ��
	int currentRow;		//��ǰ��
	int currentCol;		//��ǰ��
	MoveBy* moveBy;
	Repeat* repeate;
	Action* spawnAction;
	CallFunc* callEndGoFunc;
	Player* player;
	float** positionAroundEnd;
	
	std::vector<int> rowVector;	//������
	std::vector<int> colVector;	//������

	void moveOneStep(Player*);	//��һ��
	void handlePropEvent();             //�������
	void aroundLandEvent();             //��������
	void registerObserver();            //ע����Ϣ
	void dealMsg(Object* data);        //������Ϣ
};

#endif