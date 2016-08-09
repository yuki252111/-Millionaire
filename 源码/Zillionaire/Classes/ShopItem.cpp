#include "ShopItem.h"
#include "UDefine.h"

bool ShopItem::init()	//��ʼ���̵����
{
	if (!Sprite::init())
		return false;
	return true;
}

ShopItem* ShopItem::createShopItem(int numbers,int money, int width, int height, float CardSpriteX, float CardSpriteY,char* name1,char* name2)//�����̵����
{
	ShopItem* ShopItemInstance = new ShopItem();
	if (ShopItemInstance && ShopItemInstance->init())
	{
		ShopItemInstance->autorelease();
		ShopItemInstance->shopItemInit(numbers, money,width, height, CardSpriteX, CardSpriteY,name1,name2);

		return ShopItemInstance;
	}

	CC_SAFE_DELETE(ShopItemInstance);
	return NULL;
}

void ShopItem::shopItemInit(int numbers,int money, int width, int height, float CardSpriteX, float CardSpriteY,char* name1,char* name2)
{
	//���ó�ʼ��ֵ
	ItemNumber = numbers;
	ItemMoney = money;

	//������Ϸ�ı�����ɫ
	layerColorBG = LayerColor::create(Color4B(100, 100, 100, 255), width - 5, height - 5);
	layerColorBG->setPosition(Point(CardSpriteX, CardSpriteY));

	if (ItemNumber > 0)
	{
		ballMenuImage = MenuItemImage::create(name1, name2, this, menu_selector(ShopItem::ballButtonCallback));
		ballMenuImage->setTag(ItemNumber);
		ballMenuImage->setPosition(Point(layerColorBG->getContentSize().width / 2, layerColorBG->getContentSize().height / 2));

		// �������˵��������λ��
		labelLotteryNumber = LabelTTF::create(String::createWithFormat("%i", ItemNumber)->getCString(), "HiraKakuProN-W6", 25);
		labelLotteryNumber->setColor(Color3B(200, 200, 200));
		labelLotteryNumber->setPosition(Point(layerColorBG->getContentSize().width / 2, layerColorBG->getContentSize().height / 2));

		ballMenuImage->addChild(labelLotteryNumber);

		menu = Menu::create();
		menu->addChild(ballMenuImage);
		menu->setPosition(CCPointZero);

		layerColorBG->addChild(menu);
		this->setTag(ItemNumber);
	}
	this->addChild(layerColorBG);

}

void ShopItem::ballButtonCallback(CCObject* pSender) //ͼƬ�ص�
{
	Node* node = dynamic_cast<Node*>(pSender);
	ballMenuImage->selected();
	if (m_callback && m_callbackListener)
	{
		(m_callbackListener->*m_callback)(node);
	}
}

void ShopItem::setUnSelected()
{
	ballMenuImage->unselected();
}
void ShopItem::setSelected()
{
	ballMenuImage->selected();
}
bool ShopItem::isSelected()
{
	return ballMenuImage->isSelected();
}

void ShopItem::setBallCallbackFunc(cocos2d::Object *target, SEL_CallFuncN callfun) //���ûص�����
{
	m_callbackListener = target;
	m_callback = callfun;
}

//��ȡ����
int ShopItem::getItemNumber()
{
	return ItemNumber;
}

//��������
void ShopItem::setItemNumber(int num)
{
	ItemNumber = num;

	if (ItemNumber > 0)
	{
		//��ȡ�������¸�������
		labelLotteryNumber->setString(String::createWithFormat("%i", ItemNumber)->getCString());
	}

}