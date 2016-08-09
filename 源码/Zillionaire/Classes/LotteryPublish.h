#ifndef __LOTTERY_PUBLISH_H__
#define __LOTTERY_PUBLISH_H__

#include "cocos2d.h"

USING_NS_CC;
//¿ª²ÊÆ±
class LotteryPublish : public Sprite
{
public:
	~LotteryPublish();
	bool init();
	void addItemSpriteFrameCache();
	CREATE_FUNC(LotteryPublish);
	CC_SYNTHESIZE(Animate*, normal_anmi, Normal_anmi);

private:
	SpriteFrameCache* itemSpriteFrameCache;
	char name[20];
	Vector<SpriteFrame*> item_anim_vector;
	void setItemAnimate();

};

#endif