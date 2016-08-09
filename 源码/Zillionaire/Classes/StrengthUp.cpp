#include "StrengthUp.h"

StrengthUp::~StrengthUp()
{
	normal_anmi->release();
}

bool StrengthUp::init()
{
	addItemSpriteFrameCache();

	SpriteFrame* spf;
	spf = itemSpriteFrameCache->getSpriteFrameByName("strength_up_01.png"); //加载精灵
	Sprite::initWithSpriteFrame(spf);
	setItemAnimate();
	return true;
}

void StrengthUp::addItemSpriteFrameCache()
{
	itemSpriteFrameCache = SpriteFrameCache::getInstance();
	itemSpriteFrameCache->addSpriteFramesWithFile("strength_up.plist", "strength_up.png"); //加载动画
	itemSpriteFrameCache->retain();

	memset(name, 0, 20);

	for (int i = 1; i <= 14; i++)
	{
		sprintf(name, "strength_up_%02d.png", i);
		item_anim_vector.pushBack(itemSpriteFrameCache->getSpriteFrameByName(name));
	}

}

void StrengthUp::setItemAnimate()  //创建动画
{
	if (!AnimationCache::getInstance()->getAnimation("strength_up_animation"))
	{
		AnimationCache::getInstance()->addAnimation(Animation::createWithSpriteFrames(item_anim_vector, 0.1f), "strength_up_animation");
	}
	normal_anmi = Animate::create(AnimationCache::getInstance()->getAnimation("strength_up_animation"));
	normal_anmi->retain();
}