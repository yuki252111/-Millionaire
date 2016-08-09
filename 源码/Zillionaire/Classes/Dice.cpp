#include "Dice.h"
#include "GameMapBaseScene.h"

Dice* Dice::getInstance() //获取一个实例
{
	Dice* diceInstance = Dice::create();
	return diceInstance;
}
Dice::~Dice()	//释放内存
{
	diceFrameCache->release(); 
	diceAnim->release();
}
bool Dice::init()	//虚函数重载
{
	if (!Layer::init())
		return false;
	addDiceAnim();
	return true;
}
void Dice::addDiceAnim()   //加载色子动画
{
	//加载动画
	diceFrameCache = SpriteFrameCache::getInstance();
	diceFrameCache->addSpriteFramesWithFile(DICE_ANIM_PLIST, DICE_ANIM_PNG);
	diceFrameCache->retain();

	char name[20];
	memset(name, 0, 20);
	for (int i = 1; i <= 6; i++)
	{
		sprintf(name, "dice_%02d.png", i);
		diceVector.pushBack(diceFrameCache->getSpriteFrameByName(name));
	}

	if (!AnimationCache::getInstance()->getAnimation("dice_animation"))
	{
		AnimationCache::getInstance()->addAnimation(Animation::createWithSpriteFrames(diceVector, 0.1), "dice_animation");
	}
	diceAnim = Animate::create(AnimationCache::getInstance()->getAnimation("dice_animation"));
	diceAnim->retain();
	//加载色子精灵
	dice = Sprite::createWithSpriteFrame(diceFrameCache->getSpriteFrameByName("dice_01.png"));
	dice->setPosition(ccp(tableStartPosition_x + 2 * tableWidth, tableStartPosition_y - tableHeight*4.5));
	this->addChild(dice);
	//运行动画
	dice->runAction(RepeatForever::create(diceAnim));
}
