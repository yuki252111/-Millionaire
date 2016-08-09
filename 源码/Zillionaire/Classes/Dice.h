#ifndef __DICE_H__
#define __DICE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "UDefine.h"

USING_NS_CC;
USING_NS_CC_EXT;

class Dice :public Layer{
public:
	static Dice* getInstance();
	~Dice();
	
	virtual bool init();
	CREATE_FUNC(Dice);
	CC_SYNTHESIZE(SpriteFrameCache*, diceFrameCache, DiceFrameCache); //��������
	CC_SYNTHESIZE(Sprite* ,dice, Dice);						  //��ǰ����

private:
	Animate* diceAnim;                //����
	Vector<SpriteFrame*> diceVector;  //ÿ֡����
	void addDiceAnim();
};

#endif