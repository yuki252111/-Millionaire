#include "StockCellCard.h"

bool StockCellCard::init()	//初始化
{
	if (!Sprite::init())
	{
		return false;
	}
	return true;
}


StockCellCard* StockCellCard::createCardSprite(String* _info, int width, int height, float CardSpriteX, float CardSpriteY, int colorTag)//创建单元格
{
	StockCellCard* cellCard = new StockCellCard();
	if (cellCard && cellCard->init())
	{
		cellCard->autorelease();
		cellCard->cardInit(_info, width, height, CardSpriteX, CardSpriteY, colorTag);

		return cellCard;
	}

	CC_SAFE_DELETE(cellCard);
	return NULL;
}

void StockCellCard::cardInit(String* _info, int width, int height, float CardSpriteX, float CardSpriteY, int colorTag)
{
	info = _info;

	layerColorBG = LayerColor::create(Color4B(255, 255, 255, 255), width - 5, height - 5);	
	if (colorTag>0)
	{
		layerColorBG = LayerColor::create(Color4B(0, 255,50, 255), width - 5, height - 5);//设置背景色
	}
	else
	{
		layerColorBG = LayerColor::create(Color4B(0, 50, 255, 255), width - 5, height - 5);
	}


	layerColorBG->setPosition(Point(CardSpriteX, CardSpriteY));

	if (_info != NULL)
	{

		labelInfo = LabelTTF::create(_info->getCString(), "", 20); //设置文字说明
		labelInfo->setColor(Color3B::BLACK);
		labelInfo->setPosition(Point(layerColorBG->getContentSize().width / 2, layerColorBG->getContentSize().height / 2));
		layerColorBG->addChild(labelInfo);
	}

	this->addChild(layerColorBG);

}

void StockCellCard::setInfo(String* c)  //更改说明
{
	info = c;
	labelInfo->setString(c->getCString());

}