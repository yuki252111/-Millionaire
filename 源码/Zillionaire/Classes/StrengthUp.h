#ifndef __STRENGTH_UP_H__
#define __STRENGTH_UP_H__

#include "cocos2d.h"
USING_NS_CC;
//体力回升动画
class StrengthUp : public Sprite
{
public:
	~StrengthUp();
	bool init();
	void addItemSpriteFrameCache();
	CC_SYNTHESIZE(Animate*, normal_anmi, Normal_anmi);
	CREATE_FUNC(StrengthUp);
private:
	SpriteFrameCache* itemSpriteFrameCache;
	char name[20];
	Vector<SpriteFrame*> item_anim_vector;
	void setItemAnimate();

};

#endif