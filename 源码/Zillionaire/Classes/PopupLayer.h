#ifndef __POPUPLAYER_H__
#define __POPUPLAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Lottery.h"
#include "LotteryPublish.h"
#include "Player.h"
#include "ShopItem.h"

USING_NS_CC;
USING_NS_CC_EXT;

const int popupFontSize = 20;

enum POP_TYPE
{
	NORMAL,
	LOTTERY,
	LOTTERY_PUBLISH,
	STOCK,
	SHOP,
	PACKAGE,
};
//�����򣬰��������ضԻ������Ʊ�Ի�������Ʊ���������Ʊ������ߣ�ʹ�õ���
class PopupLayer :public Layer
{
public:
	PopupLayer();
	~PopupLayer();
	virtual bool init();
	CREATE_FUNC(PopupLayer);
	//��ͨ�Ի���
	static PopupLayer * createPopupLayer(const char* backgroundImage);
	void setTitle(const char* title, int fontsize = popupFontSize);
	void setContentText(const char* text, int fontsize = popupFontSize, int padding = 50, int paddintTop = 100);
	void setCallbackFunc(Object* target, SEL_CallFuncN callfun);

	bool addButton(const char* normalImage, const char* selectedImage, const char* title, int tag = 0);
	virtual void onEnter();
	
	//���Ʊ
	POP_TYPE pop_type;
	bool lotteryBought;
	void setPopType(POP_TYPE type);
	Vector<Lottery*> lotteryVector;
	void refreshBallBackGround(Node *pNode);
	std::vector<int> selected_number_vector;
	void setLotteryContext();
	void setHasSelectedLotteryNumber(int num);
	//����Ʊ
	void setLotteryPublishContext(Size size);
	void addPlayer(Player* player);
	void dismissFromParent(float);
	void addPlayerLottery();
	void runLotteryAnim();
	void selectedClear();
	int lott;
	//�����
	Vector<ShopItem*>shopItemVector;
	void refreshItemBackGround(Node* pnode);
	void setItemContext();
	bool bought;
	//ʹ�õ���
	std::vector<int>playerPackageTag;
	std::vector<int>playerPackageNum;
	Vector<ShopItem*>playerPackageItem;
	Vector<LabelTTF*>playerPackageLabel;
	Player* player;
	bool used;
	void setPlayerPackageContext(Player*);
	void clearPlayerPackageContext();
private:
	void buttonCallback(CCObject* pSender);
	void addPlayerInfo(Size size);
	

	// �����������ߵĿհ���
	int contentPadding;
	int contentPaddingTop;
	int lotteryMoney ;

	CCObject* callbackListener;
	SEL_CallFuncN callback;
	LotteryPublish* lp;
	Vector<Player*> players_vector;
	Vector<LabelTTF*> playerLotteryVector;

	CC_SYNTHESIZE_RETAIN(Menu*, pMenu, MenuButton);
	CC_SYNTHESIZE_RETAIN(Sprite*, sfBackGround, SpriteBackGround);
	CC_SYNTHESIZE_RETAIN(Scale9Sprite*, s9BackGround, Sprite9BackGround);
	CC_SYNTHESIZE_RETAIN(LabelTTF*, ltTitle, LabelTitle);
	CC_SYNTHESIZE_RETAIN(LabelTTF*, ltContentText, LabelContentText);

	CC_SYNTHESIZE(int, dataTag, DataTag);
	CC_SYNTHESIZE(int, playerTag, PlayerTag);
};


#endif