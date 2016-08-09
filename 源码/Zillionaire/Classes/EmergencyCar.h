#ifndef __EMERGENCY_CAR_H__
#define __EMERGENCY_CAR_H__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class EmergencyCar: public Sprite
{
public:
	~EmergencyCar();
	static EmergencyCar* createEmergencyCar();

	CC_SYNTHESIZE(Animate*, car_go_anmi, Car_go_anmi);
	CC_SYNTHESIZE(Animate*, car_stop_anmi, Car_stop_anmi);

private:
	SpriteFrameCache* itemSpriteFrameCache;
	Vector<SpriteFrame*> item_cargo_anim_vector;
	Vector<SpriteFrame*> item_carstop_anim_vector;
	char name[20];

	void setItemAnimate();
	bool initCar();
	void addItemSpriteFrameCache();
};

#endif