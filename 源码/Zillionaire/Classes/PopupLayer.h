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
//弹出框，包括买土地对话，买彩票对话，开彩票动画，买股票，买道具，使用道具
class PopupLayer :public Layer
{
public:
	PopupLayer();
	~PopupLayer();
	virtual bool init();
	CREATE_FUNC(PopupLayer);
	//普通对话框
	static PopupLayer * createPopupLayer(const char* backgroundImage);
	void setTitle(const char* title, int fontsize = popupFontSize);
	void setContentText(const char* text, int fontsize = popupFontSize, int padding = 50, int paddintTop = 100);
	void setCallbackFunc(Object* target, SEL_CallFuncN callfun);

	bool addButton(const char* normalImage, const char* selectedImage, const char* title, int tag = 0);
	virtual void onEnter();
	
	//买彩票
	POP_TYPE pop_type;
	bool lotteryBought;
	void setPopType(POP_TYPE type);
	Vector<Lottery*> lotteryVector;
	void refreshBallBackGround(Node *pNode);
	std::vector<int> selected_number_vector;
	void setLotteryContext();
	void setHasSelectedLotteryNumber(int num);
	//开彩票
	void setLotteryPublishContext(Size size);
	void addPlayer(Player* player);
	void dismissFromParent(float);
	void addPlayerLottery();
	void runLotteryAnim();
	void selectedClear();
	int lott;
	//买道具
	Vector<ShopItem*>shopItemVector;
	void refreshItemBackGround(Node* pnode);
	void setItemContext();
	bool bought;
	//使用道具
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
	

	// 文字内容两边的空白区
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