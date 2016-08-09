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
//��Ϸ����
class GameHall :public Layer{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameHall);
private:
	Vector<Button*>posButtons;//λ�ð�ť
	std::vector<Point>posPoints;	//λ�õ�
	Vector<Sprite*>players;	//�û�����
	int curpos;	//��ǰλ��
	LabelTTF* sign;	//��ǰλ��ָʾ��

	std::vector<std::string>msg;	//��Ϣ
	CRITICAL_SECTION g_cs;
	CRITICAL_SECTION m_cs;

	void onclick(Ref*,TouchEventType type);	//�ص�����
	bool connect();	//���ӷ�����
	void receiveData();	//��������
	virtual void update(float dt);	//��������
};

#endif