#include "PopupLayer.h"
#include "CocosToast.h"
#include "UDefine.h"
#include "UFunction.h"


PopupLayer::PopupLayer() :pMenu(NULL)	//构造函数
, contentPadding(0)
, contentPaddingTop(0)
, callbackListener(NULL)
, callback(NULL)
, sfBackGround(NULL)
, s9BackGround(NULL)
, ltContentText(NULL)
, ltTitle(NULL)
{

}

PopupLayer::~PopupLayer()	//析构函数，释放内存
{
	CC_SAFE_RELEASE(pMenu);
	CC_SAFE_RELEASE(sfBackGround);
	CC_SAFE_RELEASE(ltContentText);
	CC_SAFE_RELEASE(ltTitle);
	CC_SAFE_RELEASE(s9BackGround);
}

bool PopupLayer::init()	//初始化
{
	if (!Layer::init())
		return false;

	this->setContentSize(CCSizeZero);

	// 初始化需要的 Menu
	pMenu = Menu::create();
	pMenu->setPosition(CCPointZero);

	return true;
}


PopupLayer* PopupLayer::createPopupLayer(const char *backgroundImage) //普通对话创建
{
	PopupLayer* ml = PopupLayer::create();
	ml->setSpriteBackGround(Sprite::create(backgroundImage));
	ml->setSprite9BackGround(Scale9Sprite::create(backgroundImage));
	return ml;
}

void PopupLayer::setTitle(const char *title, int fontsize) //标题
{
	LabelTTF* ltfTitle = LabelTTF::create(title, "", fontsize);
	ltTitle = ltfTitle;
}

void PopupLayer::setContentText(const char *text, int fontsize, int padding, int paddingTop) //内容文本
{
	LabelTTF* ltf = LabelTTF::create(text, "", fontsize);
	ltContentText = ltf;
	contentPadding = padding;
	contentPaddingTop = paddingTop;
}

void PopupLayer::setCallbackFunc(cocos2d::Object *target, SEL_CallFuncN callfun) //设置回调
{
	callbackListener = target;
	callback = callfun;
}


bool PopupLayer::addButton(const char *normalImage, const char *selectedImage, const char *title, int tag) //加载按钮
{
	//Size winSize = CCDirector::getInstance()->getWinSize();
	Point pCenter = ccp(winSize.width / 2, winSize.height / 2);

	// 创建图片菜单按钮
	MenuItemImage* menuImage = MenuItemImage::create(normalImage, selectedImage, this, menu_selector(PopupLayer::buttonCallback));
	menuImage->setTag(tag);
	menuImage->setPosition(pCenter);

	// 添加文字说明并设置位置

	Size imenu = menuImage->getContentSize();
	LabelTTF* ttf = LabelTTF::create(title, "", 20);
	ttf->setColor(ccc3(0, 0, 0));
	ttf->setPosition(ccp(imenu.width / 2, imenu.height / 2));
	menuImage->addChild(ttf);


	pMenu->addChild(menuImage);
	return true;
}

void PopupLayer::buttonCallback(cocos2d::CCObject *pSender)	//设置按钮回调
{
	Node* node = dynamic_cast<Node*>(pSender);

	if (callback && callbackListener){
		(callbackListener->*callback)(node);
	}
}

void PopupLayer::onEnter()	//进入时调用
{
	Layer::onEnter();

	//Size winSize = Director::getInstance()->getWinSize();
	Point pCenter = ccp(winSize.width / 2, winSize.height / 2);

	Size contentSize;
	// 设定好参数，在运行时加载
	if (getContentSize().equals(CCSizeZero))
	{
		sfBackGround->setPosition(pCenter);
		this->addChild(sfBackGround, 0, 0);
		contentSize = sfBackGround->getTexture()->getContentSize();
	}
	else {
		Scale9Sprite *background = s9BackGround;
		background->setContentSize(getContentSize());
		background->setPosition(pCenter);
		this->addChild(background, 0, 0);
		contentSize = getContentSize();
	}


	// 添加按钮，并设置其位置
	this->addChild(pMenu);
	float btnWidth = contentSize.width / (pMenu->getChildrenCount() + 1);

	Vector<Node*> vecArray = getMenuButton()->getChildren();

	int j = 0;
	for (auto it = vecArray.begin(); it != vecArray.end(); it++)
	{
		Node* node = dynamic_cast<Node*>(*it);
		node->setPosition(Point(winSize.width / 2 - contentSize.width / 2 + btnWidth*(j + 1), winSize.height / 2 - contentSize.height / 3));
		j++;
	}

	// 显示对话框标题
	if (ltTitle)
	{
		ltTitle->setPosition(ccpAdd(pCenter, ccp(0, contentSize.height / 2 - 20)));
		ltTitle->setColor(ccc3(0, 0, 0));
		this->addChild(ltTitle);
	}
	switch (pop_type){
	case LOTTERY:	//买彩票
	{
		lotteryBought = false;
		setLotteryContext();
		break;
	}
	case LOTTERY_PUBLISH:	//开彩票
	{
		setLotteryPublishContext(contentSize);
		break;
	}
	case SHOP:	//商店
	{
		bought = false;
		setItemContext();
		break;
	}
	case PACKAGE:	//包裹
	{
		used = false;
		break;
	}
	default: 
	{
		// 显示文本内容
		if (ltContentText)
		{
			LabelTTF* ltf = ltContentText;
			ltf->setPosition(pCenter);
			ltf->setDimensions(CCSizeMake(contentSize.width - contentPadding * 2, contentSize.height - contentPaddingTop));
			ltf->setHorizontalAlignment(kCCTextAlignmentLeft);
			ltf->setColor(ccc3(0, 0, 0));
			this->addChild(ltf);
		}
	}
	}

	// 弹出效果
	Action* popupLayer = Sequence::create(ScaleTo::create(0.0, 0.0),
		ScaleTo::create(0.15, 1.05),
		ScaleTo::create(0.08, 0.95),
		ScaleTo::create(0.08, 1.0),
		NULL);
	this->runAction(popupLayer);

}
//----------------------------------------------------
//买彩票
void PopupLayer::setLotteryContext()
{
	//Size winSize = Director::getInstance()->getWinSize();
	Size size = getContentSize();
	Size center = (winSize - size) / 2;
	//求出单元格的宽度和高度
	int unitHeight = (size.height) / 6;
	int unitwidth = (size.width) / 5;
	//10*3的单元格
	for (int row = 0; row<10; row++)
	{
		for (int col = 0; col<3; col++)
		{

			Lottery* card = Lottery::createLottery((row + 1) + col * 10, 40, 40, center.width + 20 + row*(size.width / 11), (winSize.height / 2 + 30) - 40 * col);
			card->setTag((row + 1) + col * 10);
			card->setBallCallbackFunc(this, callfuncN_selector(PopupLayer::refreshBallBackGround));
			card->setLotteryNumber((row + 1) + col * 10);
			lotteryVector.pushBack(card);

			this->addChild(card);
		}
	}
}
void PopupLayer::refreshBallBackGround(Node* pNode) //买彩票回调
{
	int tag2 = pNode->getTag();
	lotteryBought = true;

	bool selected = true;
	for (int j = 0; j < selected_number_vector.size(); j++)
	{
		if (tag2 == selected_number_vector[j])
		{
			CocosToast::createToast(this, "Has been selected", 4.0f, pNode->getPosition());
			selected = false;
		}
	}

	for (auto it = lotteryVector.begin(); it != lotteryVector.end(); it++)
	{
		Lottery* node = (Lottery*)(*it);

		if (node->getTag() != pNode->getTag())
		{
			node->setUnSelected();
		}
	}

	Vector<Node*> menuItemVector = getMenuButton()->getChildren();
	for (int i = 0; i< getMenuButton()->getChildrenCount(); i++)	//把号码写入按钮传递
	{
		if (menuItemVector.at(i)->getTag() != 0)
		{
			if (selected)
				menuItemVector.at(i)->setTag(tag2);
			else
				menuItemVector.at(i)->setTag(-1);
			break;
		}
	}

}

void PopupLayer::setHasSelectedLotteryNumber(int num)	//设置已经被买的
{
	selected_number_vector.push_back(num);
	for (int i = 0; i < lotteryVector.size(); i++)
	{
		if (lotteryVector.at(i)->getLotteryNumber() == num)
		{
			lotteryVector.at(i)->setVisible(false);
			break;
		}
	}
}

//------------------------------------------------------------------------
void PopupLayer::setLotteryPublishContext(Size size)  //开彩票
{
	//Size winSize = Director::getInstance()->getWinSize();
	lp = LotteryPublish::create();
	this->addChild(lp);
	lp->setPosition(Point(winSize.width / 2, winSize.height / 2));
	lotteryMoney = 0;
	addPlayerInfo(size);
}
void PopupLayer::addPlayer(Player* player)
{
	players_vector.pushBack(player);
}
void PopupLayer::dismissFromParent(float dt)
{
	NotificationCenter::getInstance()->postNotification(MSG_BUY, String::createWithFormat("%d", MSG_BUY_LOTTERY_WIN_TAG));
	this->setVisible(false);
}
void PopupLayer::runLotteryAnim() //开彩票动画
{
	lotteryMoney += LOTTERY_WIN_MONEY;

	lp->runAction(Sequence::create(lp->getNormal_anmi(),
		CallFunc::create([this]()
	{
		scheduleOnce(schedule_selector(PopupLayer::dismissFromParent), 4.0f);
		Sprite* ball = Sprite::create("orange_ball.png");
		ball->setPosition((lp->getPosition()) - Point(lp->getContentSize()) / 2 + ccp(0, 13));
		ball->setAnchorPoint(ccp(0, 0));
		addChild(ball);
		LabelTTF* ltf = LabelTTF::create(String::createWithFormat("%02d", lott)->getCString(), "", 20);
		ltf->setPosition(ball->getPosition() + ccp(5, 6));
		ltf->setAnchorPoint(ccp(0, 0));
		addChild(ltf);
		
		Size center = (winSize) / 2;
		int j = 0;
		bool found = false;
		for (auto it = players_vector.begin(); it != players_vector.end(); it++)
		{
			Player* player = dynamic_cast<Player*>(*it);

			for (int i = 0; i < player->getLotteryVector().size(); i++)
			{
				if (player->getLotteryVector().at(i) == lott)
				{
					found = true;
					UFunction::playEffectMusic(PARTICLE_EFFECT, false);
					player->setMoney(player->getMoney() + lotteryMoney);

					ParticleSystem* lotteryWinParticle = ParticleSystemQuad::create("lottery_win.plist");
					lotteryWinParticle->retain();
					ParticleBatchNode *batch = ParticleBatchNode::createWithTexture(lotteryWinParticle->getTexture());
					batch->addChild(lotteryWinParticle);
					this->addChild(batch);

					lotteryWinParticle->setPosition(center.width + 20, (winSize.height / 2 + 50) + j * 50);
					lotteryWinParticle->release();
					lotteryWinParticle->setAutoRemoveOnFinish(true);

				}
			}
			player->lotteryClear();

			j++;
		}
		if (found == false)
			UFunction::playEffectMusic(P1_meirendejiang, false);
	}
	), NULL));
}
void PopupLayer::addPlayerLottery()  //用户买彩票情况
{

	for (int i = 1; i <= 30; i++)
	{
		if (this->getChildByTag(1000 + i) != NULL)
		{
			this->removeChildByTag(1000 + i);
		}

	}
	for (int i = 1; i <= 30; i++)
	{
		if (this->getChildByTag(2000 + i) != NULL)
		{
			this->removeChildByTag(2000 + i);
		}

	}

	
	Size size = this->getContentSize();
	Size center = (winSize - size) / 2;
	int j = 0;
	for (auto it = players_vector.begin(); it != players_vector.end(); it++)
	{
		Player* player = dynamic_cast<Player*>(*it);

		playerLotteryVector.clear();
		for (int i = 0; i < player->getLotteryVector().size(); i++)
		{
			Sprite* sp = Sprite::create("orange_ball.png");
			sp->setPosition(ccp(center.width + 20 + (i + 1) * 20, (winSize.height / 2 + 30) + j * 50));
			sp->setTag(2000 + player->getLotteryVector().at(i));
			this->addChild(sp);

			LabelTTF* labelLotteryNumber = LabelTTF::create(String::createWithFormat("%i", player->getLotteryVector().at(i))->getCString(), "", 15);
			labelLotteryNumber->setPosition(ccp(center.width + 20 + (i + 1) * 20, (winSize.height / 2 + 30) + j * 50));
			labelLotteryNumber->setColor(Color3B::BLACK);
			labelLotteryNumber->setTag(1000 + player->getLotteryVector().at(i));

			playerLotteryVector.pushBack(labelLotteryNumber);
		}

		for (int i = 0; i < playerLotteryVector.size(); i++)
		{
			this->addChild(playerLotteryVector.at(i), 1);
		}
		j++;

	}
}
void PopupLayer::addPlayerInfo(Size size)  //角色信息
{
	//Size winSize = Director::getInstance()->getWinSize();
	Size center = (winSize - size) / 2;
	int j = 0;
	for (auto it = players_vector.begin(); it != players_vector.end(); it++)
	{
		Player* player = dynamic_cast<Player*>(*it);

		SpriteFrame* spf;
		int tag = player->getTag();
		switch (tag)
		{
		case PLAYER_1_TAG:
		{
			spf = player->getPlayerSpriteFrameCache()->getSpriteFrameByName("player1_anim_01.png");
			break;
		}
		case PLAYER_2_TAG:
		{
			spf = player->getPlayerSpriteFrameCache()->getSpriteFrameByName("player2_anim_02.png");
			break;
		}
		}

		Sprite* playerSprite = Sprite::createWithSpriteFrame(spf);
		playerSprite->setPosition(center.width + 20, (winSize.height / 2 + 50) + j * 50);
		this->addChild(playerSprite);
		j++;
	}
}
//--------------------------------------------------------------------------
void PopupLayer::setItemContext()  //买道具
{
	//Size winSize = Director::getInstance()->getWinSize();
	Size size = getContentSize();
	Size center = (winSize - size) / 2;
	//求出单元格的宽度和高度
	int unitHeight = (size.height) / 6;
	int unitwidth = (size.width) / 5;
	int row = 0;
	for (; row<6; row++)
	{
		ShopItem* card = ShopItem::createShopItem((row + 1), (row + 1)*BUY_SHOP_ITEM_MONEY, 40, 55, center.width + 20 + row*(size.width / 6), (winSize.height / 2 + 10), "step_item_normal.jpg", "step_item_selected.jpg");
		card->setTag((row + 1));
		card->setBallCallbackFunc(this, callfuncN_selector(PopupLayer::refreshItemBackGround));
		card->setItemNumber((row + 1));
		shopItemVector.pushBack(card);

		LabelTTF* money = LabelTTF::create(String::createWithFormat("$%i", card->getitemMoney())->getCString(), "HiraKakuProN-W6", 15);
		money->setColor(Color3B::YELLOW);
		money->setPosition(Point(center.width + 30 + row*(size.width / 6), (winSize.height / 2)));

		this->addChild(card);
		this->addChild(money);
	}
	ShopItem* card = ShopItem::createShopItem((row + 1), (row + 1)*BUY_SHOP_ITEM_MONEY, 40, 55, center.width + 20 + (row-6)*(size.width / 6), (winSize.height / 2+10 -60),"storm_item_normal.jpg","storm_item_selected.jpg");
	card->setTag((row + 1));
	card->setBallCallbackFunc(this, callfuncN_selector(PopupLayer::refreshItemBackGround));
	card->setItemNumber((row + 1));
	shopItemVector.pushBack(card);

	LabelTTF* money = LabelTTF::create(String::createWithFormat("$%i", card->getitemMoney())->getCString(), "HiraKakuProN-W6", 15);
	money->setColor(Color3B::YELLOW);
	money->setPosition(Point(center.width + 30 + (row-6)*(size.width / 6), (winSize.height / 2-10-60)));

	this->addChild(card);
	this->addChild(money);
	row++;

	card = ShopItem::createShopItem((row + 1), (row + 1)*BUY_SHOP_ITEM_MONEY, 40, 55, center.width + 20 + (row - 6)*(size.width / 6), (winSize.height / 2 + 10 - 60), "toll_item_normal.jpg", "toll_item_selected.jpg");
	card->setTag((row + 1));
	card->setBallCallbackFunc(this, callfuncN_selector(PopupLayer::refreshItemBackGround));
	card->setItemNumber((row + 1));
	shopItemVector.pushBack(card);

	money = LabelTTF::create(String::createWithFormat("$%i", card->getitemMoney())->getCString(), "HiraKakuProN-W6", 15);
	money->setColor(Color3B::YELLOW);
	money->setPosition(Point(center.width + 30 + (row - 6)*(size.width / 6), (winSize.height / 2 - 10 - 60)));

	this->addChild(card);
	this->addChild(money);
}
void PopupLayer::refreshItemBackGround(Node* pNode)  //买道具回调
{
	int tag2 = pNode->getTag();
	bought = true;
	used = true;

	for (auto it = shopItemVector.begin(); it != shopItemVector.end(); it++)
	{
		ShopItem* node = (ShopItem*)(*it);

		if (node->getTag() != pNode->getTag())
		{
			node->setUnSelected();
		}
	}

	Vector<Node*> menuItemVector = getMenuButton()->getChildren();
	for (int i = 0; i< getMenuButton()->getChildrenCount(); i++)
	{
		if (menuItemVector.at(i)->getTag() != 0)
		{
			menuItemVector.at(i)->setTag(tag2);
			break;
		}
	}
}
//------------------------------------------------------------------------
void PopupLayer::setPlayerPackageContext(Player* currentPlayer)  //使用道具
{
	player = currentPlayer;
	playerPackageNum.clear();
	playerPackageTag.clear();
	std::map<int, int>::iterator iter = player->shopItemMap.begin();
	while (iter != player->shopItemMap.end())
	{
		playerPackageTag.push_back(iter->first);
		playerPackageNum.push_back(iter->second);
		iter++;
	}
	
	
	//Size winSize = Director::getInstance()->getWinSize();
	Size size = getContentSize();
	Size center = (winSize - size) / 2;
	//求出单元格的宽度和高度
	int unitHeight = (size.height) / 6;
	int unitwidth = (size.width) / 5;

	
	for (int row = 0; row<playerPackageTag.size(); row++)
	{
		if (playerPackageTag[row] <= 6)
		{
			ShopItem* card = ShopItem::createShopItem(playerPackageTag[row], 0, 40, 55, center.width + 20 + row*(size.width / 6), (winSize.height / 2 + 10),"step_item_normal.jpg","step_item_selected.jpg");
			card->setTag(playerPackageTag[row]);
			card->setBallCallbackFunc(this, callfuncN_selector(PopupLayer::refreshItemBackGround));
			card->setItemNumber(playerPackageTag[row]);
			playerPackageItem.pushBack(card);

			LabelTTF* num = LabelTTF::create(String::createWithFormat("X%i", playerPackageNum[row])->getCString(), "HiraKakuProN-W6", 20);
			num->setColor(Color3B::BLACK);
			num->setPosition(Point(center.width + 40 + row*(size.width / 6), (winSize.height / 2)));
			playerPackageLabel.pushBack(num);

			this->addChild(card);
			this->addChild(num);
		}
		else
		{
			if (playerPackageTag[row]==7)
			{
				ShopItem* card = ShopItem::createShopItem(playerPackageTag[row], 0, 40, 55, center.width + 20 + (playerPackageTag[row]-7) * (size.width / 6), (winSize.height / 2 + 10 - 60), "storm_item_normal.jpg", "storm_item_selected.jpg");
				card->setTag(playerPackageTag[row]);
				card->setBallCallbackFunc(this, callfuncN_selector(PopupLayer::refreshItemBackGround));
				card->setItemNumber(playerPackageTag[row]);
				playerPackageItem.pushBack(card);

				LabelTTF* num = LabelTTF::create(String::createWithFormat("X%i", playerPackageNum[row])->getCString(), "HiraKakuProN-W6", 20);
				num->setColor(Color3B::BLACK);
				num->setPosition(Point(center.width + 40 + (playerPackageTag[row] - 7)*(size.width / 6), (winSize.height / 2) - 60));
				playerPackageLabel.pushBack(num);

				this->addChild(card);
				this->addChild(num);
				break;
			
			}
			if (playerPackageTag[row] == 8)
			{
				ShopItem* card = ShopItem::createShopItem(playerPackageTag[row], 0, 40, 55, center.width + 20 + (playerPackageTag[row] - 7)*(size.width / 6), (winSize.height / 2 + 10 - 60), "toll_item_normal.jpg", "toll_item_selected.jpg");
				card->setTag(playerPackageTag[row]);
				card->setBallCallbackFunc(this, callfuncN_selector(PopupLayer::refreshItemBackGround));
				card->setItemNumber(playerPackageTag[row]);
				playerPackageItem.pushBack(card);

				LabelTTF* num = LabelTTF::create(String::createWithFormat("X%i", playerPackageNum[row])->getCString(), "HiraKakuProN-W6", 20);
				num->setColor(Color3B::BLACK);
				num->setPosition(Point(center.width + 40 + (playerPackageTag[row] - 7)*(size.width / 6), (winSize.height / 2) - 60));
				playerPackageLabel.pushBack(num);

				this->addChild(card);
				this->addChild(num);
				break;

			}
		}
	}
}
void PopupLayer::clearPlayerPackageContext()	//清楚包裹
{
	for (int i = 0; i < playerPackageItem.size(); i++)
		playerPackageItem.at(i)->removeFromParent();
	for (int i = 0; i < playerPackageLabel.size(); i++)
		playerPackageLabel.at(i)->removeFromParent();
	playerPackageItem.clear();
	playerPackageLabel.clear();
	playerPackageNum.clear();
	playerPackageTag.clear();
}

void PopupLayer::setPopType(POP_TYPE pt)	//设置类型
{
	this->pop_type = pt;
}
void PopupLayer::selectedClear()	//清除已选
{
	selected_number_vector.clear();
	for (int i = 0; i < lotteryVector.size(); i++)
		lotteryVector.at(i)->setVisible(true);
}