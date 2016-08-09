#include "EmergencyCar.h"

EmergencyCar::~EmergencyCar()	//�ͷ��ڴ�
{
	car_go_anmi->release();
	car_stop_anmi->release();
}
EmergencyCar* EmergencyCar::createEmergencyCar()	//�����Ȼ���
{
	EmergencyCar* car = new EmergencyCar();
	car->initCar();
	car->autorelease();
	return car;
}
bool EmergencyCar::initCar()	//��ʼ���Ȼ���
{
	memset(name, 0, 20);
	addItemSpriteFrameCache();
	SpriteFrame* spf;
	spf = itemSpriteFrameCache->getSpriteFrameByName("car_01.png");
	Sprite::initWithSpriteFrame(spf);
	setItemAnimate();
	
	return true;
}
void EmergencyCar::addItemSpriteFrameCache()	//���ض���֡
{
	itemSpriteFrameCache = SpriteFrameCache::getInstance();
	itemSpriteFrameCache->addSpriteFramesWithFile("car.plist", "car.png");

	memset(name, 0, 20);
	for (int i = 1; i <= 2; i++)
	{
		sprintf(name, "car_%02d.png", i);
		item_cargo_anim_vector.pushBack(itemSpriteFrameCache->getSpriteFrameByName(name));
	}
	for (int i = 3; i <= 4; i++)
	{
		sprintf(name, "car_%02d.png", i);
		item_carstop_anim_vector.pushBack(itemSpriteFrameCache->getSpriteFrameByName(name));
	}
	
}
void EmergencyCar::setItemAnimate()	//���ض���
{
	if (!AnimationCache::getInstance()->getAnimation("car_go_animation"))
	{
		AnimationCache::getInstance()->addAnimation(Animation::createWithSpriteFrames(item_cargo_anim_vector, 0.1f), "car_go_animation");
	}
	car_go_anmi = Animate::create(AnimationCache::getInstance()->getAnimation("car_go_animation"));
	car_go_anmi->retain();

	if (!AnimationCache::getInstance()->getAnimation("car_stop_animation"))
	{
		AnimationCache::getInstance()->addAnimation(Animation::createWithSpriteFrames(item_carstop_anim_vector, 0.5f), "car_stop_animation");
	}
	car_stop_anmi = Animate::create(AnimationCache::getInstance()->getAnimation("car_stop_animation"));
	car_stop_anmi->retain();
}