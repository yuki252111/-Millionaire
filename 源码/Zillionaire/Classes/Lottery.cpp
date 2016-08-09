#include "Lottery.h"

bool Lottery::init()
{
	if (!Sprite::init())
		return false;
	return true;
}


Lottery* Lottery::createLottery(int numbers, int width, int height, float CardSpriteX, float CardSpriteY)
{
	Lottery* lotteryInstance = new Lottery();
	if (lotteryInstance && lotteryInstance->init())
	{
		lotteryInstance->autorelease();
		lotteryInstance->lotteryInit(numbers, width, height, CardSpriteX, CardSpriteY);

		return lotteryInstance;
	}

	CC_SAFE_DELETE(lotteryInstance);
	return NULL;
}

void Lottery::lotteryInit(int numbers, int width, int height, float CardSpriteX, float CardSpriteY)
{
	//设置初始化值
	lotteryNumber = numbers;

	//加入游戏的背景颜色
	layerColorBG = LayerColor::create(Color4B(100, 100, 100, 255), width - 5, height - 5);
	layerColorBG->setPosition(Point(CardSpriteX, CardSpriteY));

	if (lotteryNumber > 0)
	{
		ballMenuImage = MenuItemImage::create("defalut_ball.png", "blue_ball.png", this, menu_selector(Lottery::ballButtonCallback));
		ballMenuImage->setTag(lotteryNumber);
		ballMenuImage->setPosition(Point(layerColorBG->getContentSize().width / 2, layerColorBG->getContentSize().height / 2));

		// 添加文字说明并设置位置
		labelLotteryNumber = LabelTTF::create(String::createWithFormat("%i", lotteryNumber)->getCString(), "HiraKakuProN-W6", 25);
		labelLotteryNumber->setColor(Color3B(200, 200, 200));
		labelLotteryNumber->setPosition(Point(layerColorBG->getContentSize().width / 2, layerColorBG->getContentSize().height / 2));

		ballMenuImage->addChild(labelLotteryNumber);

		menu = Menu::create();
		menu->addChild(ballMenuImage);
		menu->setPosition(CCPointZero);
		
		layerColorBG->addChild(menu);
		this->setTag(lotteryNumber);
	}
	this->addChild(layerColorBG);

}

void Lottery::ballButtonCallback(CCObject* pSender)	//回调函数
{
	Node* node = dynamic_cast<Node*>(pSender);
	ballMenuImage->selected();
	if (m_callback && m_callbackListener)
	{
		(m_callbackListener->*m_callback)(node);
	}
}

void Lottery::setUnSelected()
{
	ballMenuImage->unselected();
}
void Lottery::setSelected()
{
	ballMenuImage->selected();
}
bool Lottery::isSelected()
{
	return ballMenuImage->isSelected();
}

void Lottery::setBallCallbackFunc(cocos2d::Object *target, SEL_CallFuncN callfun)	//设置回调函数
{
	m_callbackListener = target;
	m_callback = callfun;
}

//获取数字
int Lottery::getLotteryNumber()
{
	return lotteryNumber;
}

//设置数字
void Lottery::setLotteryNumber(int num)
{
	lotteryNumber = num;

	if (lotteryNumber > 0)
	{
		//获取数字重新更新数字
		labelLotteryNumber->setString(String::createWithFormat("%i", lotteryNumber)->getCString());
	}

}