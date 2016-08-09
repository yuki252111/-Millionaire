#ifndef __SHOP_ITEM_H__
#define __SHOP_ITEM_H__

#include "cocos2d.h"
USING_NS_CC;

class ShopItem : public Sprite
{
public:
	//��ʼ������ֵ����ȣ��߶ȣ���ƬX�ᣬ��ƬY��
	static ShopItem* createShopItem(int numbers,int money, int width, int height, float CardSpriteX, float CardSpriteY,char* name1,char* name2);
	virtual bool init();
	CREATE_FUNC(ShopItem);
	CC_SYNTHESIZE(int ,ItemMoney, itemMoney);

	//��ȡ����
	int getItemNumber();
	void setItemNumber(int num);

	void ballButtonCallback(CCObject* pSender);
	void setBallCallbackFunc(Object* target, SEL_CallFuncN callfun);
	void setUnSelected();
	void setSelected();
	bool isSelected();

private:
	int ItemNumber;
	void shopItemInit(int numbers, int money,int width, int height, float CardSpriteX, float CardSpriteY,char* name1,char* name2);

	MenuItemImage* ballMenuImage;
	LabelTTF* labelLotteryNumber;
	LabelTTF* labelItemMoney;
	Menu* menu;
	LayerColor* layerColorBG;

	CCObject* m_callbackListener;
	SEL_CallFuncN m_callback;
};

#endif