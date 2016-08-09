#include "LotteryPublish.h"


LotteryPublish::~LotteryPublish()	//释放内存
{
	normal_anmi->release();
	itemSpriteFrameCache->release();
}

bool LotteryPublish::init()	//初始化开彩票
{
	addItemSpriteFrameCache();
	SpriteFrame* spf;
	spf = itemSpriteFrameCache->getSpriteFrameByName("publish_ly01.png");

	Sprite::initWithSpriteFrame(spf);                 //设置精灵
	setItemAnimate();
	return true;
}

void LotteryPublish::addItemSpriteFrameCache()	//加载动画帧
{
	itemSpriteFrameCache = SpriteFrameCache::getInstance();
	itemSpriteFrameCache->addSpriteFramesWithFile("publish_ly.plist", "publish_ly.png"); //加载动画
	itemSpriteFrameCache->retain();

	memset(name, 0, 20);

	for (int i = 1; i <= 21; i++)
	{
		sprintf(name, "publish_ly%02d.png", i);
		item_anim_vector.pushBack(itemSpriteFrameCache->getSpriteFrameByName(name));
	}

}

void LotteryPublish::setItemAnimate() //创建动画
{
	if (!AnimationCache::getInstance()->getAnimation("publish_ly_animation"))
	{
		AnimationCache::getInstance()->addAnimation(Animation::createWithSpriteFrames(item_anim_vector, 0.1f), "publish_ly_animation");
	}
	normal_anmi = Animate::create(AnimationCache::getInstance()->getAnimation("publish_ly_animation"));
	normal_anmi->retain();
}
