#ifndef __STOCK_CELL_CARD_H__
#define __STOCK_CELL_CARD_H__

#include "cocos2d.h"
USING_NS_CC;
//股票单元格
class StockCellCard : public Sprite
{
public:
	static StockCellCard* createCardSprite(String* _info, int width, int height, float CardSpriteX, float CardSpriteY, int colorTag);
	virtual bool init();

	void setInfo(String* c);
	LayerColor* layerColorBG;
	LabelTTF* labelInfo;
private:

	String* info;
	MenuItemImage* ballMenuImage;

	CCObject* m_callbackListener;
	SEL_CallFuncN m_callback;

	void cardInit(String* _info, int width, int height, float CardSpriteX, float CardSpriteY, int colorTag);

	
};

#endif 