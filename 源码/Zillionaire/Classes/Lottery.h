#ifndef __LOTTERY_H__
#define __LOTTERY_H__

#include "cocos2d.h"
USING_NS_CC;

class Lottery : public Sprite
{
public:
	//��ʼ������ֵ����ȣ��߶ȣ���ƬX�ᣬ��ƬY��
	static Lottery* createLottery(int numbers, int width, int height, float CardSpriteX, float CardSpriteY);
	virtual bool init();
	CREATE_FUNC(Lottery);

	//��ȡ����
	int getLotteryNumber();
	void setLotteryNumber(int num);

	void ballButtonCallback(CCObject* pSender);
	void setBallCallbackFunc(Object* target, SEL_CallFuncN callfun);
	void setUnSelected();
	void setSelected();
	bool isSelected();
private:
	int lotteryNumber;
	void lotteryInit(int numbers, int width, int height, float CardSpriteX, float CardSpriteY);
	
	MenuItemImage* ballMenuImage;
	LabelTTF* labelLotteryNumber;
	Menu* menu;
	LayerColor* layerColorBG;

	CCObject* m_callbackListener;
	SEL_CallFuncN m_callback; //�ص�����
};

#endif