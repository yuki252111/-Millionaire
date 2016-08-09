#include "LineChart.h"
#include "GameMapBaseScene.h"

bool LineChart::initChart(Player* player)	//创建股票
{
	if (!Layer::init())
	{
		return false;
	}
	//加载背景
	bgSprite = Sprite::create("stock_bg.jpg");
	bgSprite->setPosition(Point(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height/2));
	this->addChild(bgSprite);

	richerPlayer = player;
	playerStockMap = player->stockMap;
	initStockVector(playerStockMap); //设置股票单元
	drawNode = DrawNode::create();
	this->addChild(drawNode);

	tv = TableView::create(this, Size(650, 160));
	tv->setAnchorPoint(Point(0, 0));
	tv->setPosition(10, VisibleRect::getVisibleRect().size.height * 1 / 2);

	tv->setDelegate(this);
	this->addChild(tv);
	initMenu();  //设置菜单
	

	selectedTag = 0;
	float tableY = VisibleRect::getVisibleRect().size.height * 1 / 2;
	arrowSprite_left->setPosition(600 + arrowSprite_left->getContentSize().width, tableY + selectedTag * 32);
	arrowSprite_right->setPosition(10, tableY + selectedTag * 32);

	setData(getsock_pointVec(selectedTag)); //描点
	drawpic();             //画曲线图

	return true;
}


LineChart* LineChart::createChart(Player* player)	//创建股票
{
	LineChart* mLineChart = new LineChart();
	mLineChart->initChart(player);
	mLineChart->autorelease();
	return mLineChart;
}


void LineChart::initMenu()
{//购买菜单
	Menu* menu = Menu::create();
	menu->setPosition(CCPointZero);
	_menu = menu;
	MenuItemImage* buyMenuItemButton = MenuItemImage::create("buy_normal.png", "buy_pressed.png", this, menu_selector(LineChart::buttonCallback));

	buyMenuItemButton->setPosition(ccp(700, VisibleRect::getVisibleRect().size.height - 110));
	buyMenuItemButton->setAnchorPoint(ccp(0, 0));
	buyMenuItemButton->setTag(buy_button);
	menu->addChild(buyMenuItemButton);

	//卖出菜单
	MenuItemImage* sellMenuItemButton = MenuItemImage::create("sell_normal.png", "sell_pressed.png", this, menu_selector(LineChart::buttonCallback));

	sellMenuItemButton->setPosition(ccp(700, VisibleRect::getVisibleRect().size.height - 180));
	sellMenuItemButton->setAnchorPoint(ccp(0, 0));
	sellMenuItemButton->setTag(sell_button);
	menu->addChild(sellMenuItemButton);

	//return菜单
	MenuItemImage* backMenuItemButton = MenuItemImage::create("back_normal.png", "back_pressed.png", this, menu_selector(LineChart::buttonCallback));

	backMenuItemButton->setPosition(ccp(700, VisibleRect::getVisibleRect().size.height - 250));
	backMenuItemButton->setAnchorPoint(ccp(0, 0));
	backMenuItemButton->setTag(back_button);
	menu->addChild(backMenuItemButton);

	this->addChild(menu);

	//左箭头
	arrowSprite_left = Sprite::create("arrow_left.png");
	arrowSprite_left->setPosition(ccp(-500, -500));
	arrowSprite_left->setAnchorPoint(ccp(0, 0));
	this->addChild(arrowSprite_left);
	//右箭头
	arrowSprite_right = Sprite::create("arrow_right.png");
	arrowSprite_right->setPosition(ccp(-500, -500));
	arrowSprite_right->setAnchorPoint(ccp(0, 0));
	this->addChild(arrowSprite_right);
}

void LineChart::initStockVector(Map<int, Stock*> stockMap)	//初始化股票
{
	float percent = 0;
	if (GameMapBaseScene::stock_pointvec1.size()>1)
	{
		percent = (GameMapBaseScene::stock_pointvec1.at(GameMapBaseScene::stock_pointvec1.size() - 1) - GameMapBaseScene::stock_pointvec1.at(GameMapBaseScene::stock_pointvec1.size() - 2)) / GameMapBaseScene::stock_pointvec1.at(GameMapBaseScene::stock_pointvec1.size() - 2) * 100;
	}

	stockVector.pushBack(Stock::create(800100, String::create("TECH"), GameMapBaseScene::stock_pointvec1.at(GameMapBaseScene::stock_pointvec1.size() - 1), stockMap.at(0)->getMakedealprice(), percent, stockMap.at(0)->getStoreNumber()));

	percent = 0;
	if (GameMapBaseScene::stock_pointvec2.size()>1)
	{
		percent = (GameMapBaseScene::stock_pointvec2.at(GameMapBaseScene::stock_pointvec2.size() - 1) - GameMapBaseScene::stock_pointvec2.at(GameMapBaseScene::stock_pointvec2.size() - 2)) / GameMapBaseScene::stock_pointvec2.at(GameMapBaseScene::stock_pointvec2.size() - 2) * 100;
	}

	stockVector.pushBack(Stock::create(800200, String::create("OIL"), GameMapBaseScene::stock_pointvec2.at(GameMapBaseScene::stock_pointvec1.size() - 1), stockMap.at(1)->getMakedealprice(), percent, stockMap.at(1)->getStoreNumber()));

	percent = 0;
	if (GameMapBaseScene::stock_pointvec3.size()>1)
	{
		percent = (GameMapBaseScene::stock_pointvec3.at(GameMapBaseScene::stock_pointvec3.size() - 1) - GameMapBaseScene::stock_pointvec3.at(GameMapBaseScene::stock_pointvec3.size() - 2)) / GameMapBaseScene::stock_pointvec3.at(GameMapBaseScene::stock_pointvec3.size() - 2) * 100;
	}
	stockVector.pushBack(Stock::create(800300, String::create("BANK"), GameMapBaseScene::stock_pointvec3.at(GameMapBaseScene::stock_pointvec1.size() - 1), stockMap.at(2)->getMakedealprice(), percent, stockMap.at(2)->getStoreNumber()));

	percent = 0;
	if (GameMapBaseScene::stock_pointvec4.size()>1)
	{
		percent = (GameMapBaseScene::stock_pointvec4.at(GameMapBaseScene::stock_pointvec4.size() - 1) - GameMapBaseScene::stock_pointvec4.at(GameMapBaseScene::stock_pointvec4.size() - 2)) / GameMapBaseScene::stock_pointvec4.at(GameMapBaseScene::stock_pointvec4.size() - 2) * 100;
	}
	stockVector.pushBack(Stock::create(800400, String::create("LAND"), GameMapBaseScene::stock_pointvec4.at(GameMapBaseScene::stock_pointvec1.size() - 1), stockMap.at(3)->getMakedealprice(), percent, stockMap.at(3)->getStoreNumber()));

	percent = 0;
	if (GameMapBaseScene::stock_pointvec5.size()>1)
	{
		percent = (GameMapBaseScene::stock_pointvec5.at(GameMapBaseScene::stock_pointvec5.size() - 1) - GameMapBaseScene::stock_pointvec5.at(GameMapBaseScene::stock_pointvec5.size() - 2)) / GameMapBaseScene::stock_pointvec5.at(GameMapBaseScene::stock_pointvec5.size() - 2) * 100;
	}
	stockVector.pushBack(Stock::create(800500, String::create("HOUSE"), GameMapBaseScene::stock_pointvec5.at(GameMapBaseScene::stock_pointvec1.size() - 1), stockMap.at(4)->getMakedealprice(), percent, stockMap.at(4)->getStoreNumber()));

	//添加单元项
	Label* code = Label::createWithSystemFont("Code", "", 20);
	code->setPosition(Point(20, 410));
	code->setAnchorPoint(ccp(0, 0));
	addChild(code);

	Label* name = Label::createWithSystemFont("Name", "", 20);
	name->setPosition(Point(stockCellWidth + 20, 410));
	name->setAnchorPoint(ccp(0, 0));
	addChild(name);

	Label* nowprice = Label::createWithSystemFont("Nowprice", "", 20);
	nowprice->setPosition(Point(stockCellWidth * 2 + 20, 410));
	nowprice->setAnchorPoint(ccp(0, 0));
	addChild(nowprice);

	Label* dealprice = Label::createWithSystemFont("Dealprice", "", 20);
	dealprice->setPosition(Point(stockCellWidth * 3 + 20, 410));
	dealprice->setAnchorPoint(ccp(0, 0));
	addChild(dealprice);

	Label* percentLabel = Label::createWithSystemFont("Percent", "", 20);
	percentLabel->setPosition(Point(stockCellWidth * 4 + 20, 410));
	percentLabel->setAnchorPoint(ccp(0, 0));
	addChild(percentLabel);


	Label* store = Label::createWithSystemFont("Quantity", "", 20);
	store->setPosition(Point(540, 410));
	store->setAnchorPoint(ccp(0, 0));
	addChild(store);


	playerMoneyLabel = Label::createWithSystemFont(
		String::createWithFormat("%s %d", "Playermoney", richerPlayer->getMoney())->getCString(),
		"", 20);
	playerMoneyLabel->setPosition(Point(20, 450));
	playerMoneyLabel->setAnchorPoint(ccp(0, 0));
	addChild(playerMoneyLabel);

}

void LineChart::buttonCallback(CCObject* pSender)	//回调函数
{
	int tag = ((Node*)pSender)->getTag();
	switch (tag)
	{
	case buy_button:  //买股票
	{
		int diffMoney = richerPlayer->getMoney() - stockVector.at(selectedTag)->getNowPrice() * 10;
		if (diffMoney >= 0)
		{
			Stock* s = playerStockMap.at(selectedTag);
			int storeNumber = s->getStoreNumber() + 10;

			int dealPrice = (s->getMakedealprice()*s->getStoreNumber() + stockVector.at(selectedTag)->getNowPrice() * 10) / (10 + s->getStoreNumber());

			stockVector.at(selectedTag)->setStoreNumber(storeNumber);
			Vector<Node*> cellCardVector = tv->cellAtIndex(selectedTag)->getChildren();
			StockCellCard* sdStore = (StockCellCard*)cellCardVector.at(cellCardVector.size() - 1);
			sdStore->labelInfo->setString(String::createWithFormat("%d", storeNumber)->getCString());

			StockCellCard* sdDealPrice = (StockCellCard*)cellCardVector.at(cellCardVector.size() - 3);
			sdDealPrice->labelInfo->setString(String::createWithFormat("%d", dealPrice)->getCString());


			richerPlayer->setMoney(diffMoney);
			s->setStoreNumber(s->getStoreNumber() + 10);
			s->setMakedealprice(dealPrice);

			playerMoneyLabel->setString(String::createWithFormat("%s %d", "playerMoney", richerPlayer->getMoney())->getCString());


		}
		else
		{
			CocosToast::createToast(this, String::create("noMoneyBuyStock")->_string, 2.0f,Point(VisibleRect::getVisibleRect().size / 2));
		}



		break;
	}
	case sell_button:  //卖股票
	{
		Stock* s = playerStockMap.at(selectedTag);
		int storeNumber = s->getStoreNumber();
		if (storeNumber > 0)
		{
			Vector<Node*> cellCardVector = tv->cellAtIndex(selectedTag)->getChildren();
			StockCellCard* sdStore = (StockCellCard*)cellCardVector.at(cellCardVector.size() - 1);
			sdStore->labelInfo->setString(String::createWithFormat("%d", 0)->getCString());

			StockCellCard* sdDealPrice = (StockCellCard*)cellCardVector.at(cellCardVector.size() - 3);
			sdDealPrice->labelInfo->setString(String::createWithFormat("%d", 0)->getCString());


			richerPlayer->setMoney(richerPlayer->getMoney() + storeNumber * stockVector.at(selectedTag)->getNowPrice());
			s->setMakedealprice(0);
			s->setStoreNumber(0);
			

			playerMoneyLabel->setString(String::createWithFormat("%s %d", "playerMoney", richerPlayer->getMoney())->getCString());

		}
		break;
	}
	case back_button: //返回
	{
		this->removeFromParent();
		NotificationCenter::getInstance()->postNotification(MSG_BUY, String::createWithFormat("%d", MSG_BUY_STOCK_DISMISS_TAG));
		break;
	}
	}


}
Size LineChart::cellSizeForTable(cocos2d::extension::TableView *table)
{
	return Size(100, 32);
}
void LineChart::tableCellTouched(cocos2d::extension::TableView *table, cocos2d::extension::TableViewCell *cell)//单元格回调
{

	for (int i = 0; i<30; i++)
	{
		this->removeChildByTag(100 + i);
	}

	int tag = cell->getTag();
	selectedTag = tag;
	
	float height = VisibleRect::getVisibleRect().size.height;
	float tableY = VisibleRect::getVisibleRect().size.height * 1 / 2;
	arrowSprite_left->setPosition(600 + arrowSprite_left->getContentSize().width, tableY + tag * 32);
	arrowSprite_right->setPosition(10, tableY + tag * 32);
	
	setData(getsock_pointVec(tag));	//设置数据
	drawpic();	//画线

}

vector<float> LineChart::getsock_pointVec(int id) //获取点
{
	switch (id)
	{
	case 0:
	{
		return GameMapBaseScene::stock_pointvec1;
		break;
	}

	case 1:
	{
		return GameMapBaseScene::stock_pointvec2;
		break;
	}
	case 2:
	{
		return GameMapBaseScene::stock_pointvec3;
		break;
	}
	case 3:
	{
		return GameMapBaseScene::stock_pointvec4;
		break;
	}
	case 4:
	{
		return GameMapBaseScene::stock_pointvec5;
		break;
	}

	}
	return  GameMapBaseScene::stock_pointvec1;
}

TableViewCell* LineChart::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)//画单元格
{
	TableViewCell *cell = table->dequeueCell();
	LabelTTF *label;

	int colorTag = 0;
	if (stockVector.at(idx)->getPercent()>0)
	{
		colorTag = 1;
	}
	else
	{
		colorTag = -1;
	}

	if (cell == NULL)
	{
		cell = TableViewCell::create();
		cell->setTag(idx);
		for (int i = 0; i<6; i++)	//判断单元格类型
		{
			switch (i)	
			{
			case 0:
			{
				StockCellCard* card = StockCellCard::createCardSprite(String::createWithFormat("%d", stockVector.at(idx)->getCode()), stockCellWidth, stockCellHeight, stockCellWidth*i + 10, 0, colorTag);
				cell->addChild(card);
				break;
			}
			case 1:
			{
				StockCellCard* card = StockCellCard::createCardSprite(stockVector.at(idx)->getStockName(), stockCellWidth, stockCellHeight, stockCellWidth*i + 10, 0, colorTag);
				cell->addChild(card);
				break;
			}
			case 2:
			{
				StockCellCard* card = StockCellCard::createCardSprite(String::createWithFormat("%d", stockVector.at(idx)->getNowPrice()), stockCellWidth, stockCellHeight, stockCellWidth*i + 10, 0, colorTag);
				cell->addChild(card);
				break;
			}

			case 3:
			{
				StockCellCard* card = StockCellCard::createCardSprite(String::createWithFormat("%d", stockVector.at(idx)->getMakedealprice()), stockCellWidth, stockCellHeight, stockCellWidth*i + 10, 0, colorTag);
				cell->addChild(card);
				break;
			}
			case 4:
			{

				StockCellCard* card = StockCellCard::createCardSprite(String::createWithFormat("%.0f %s", stockVector.at(idx)->getPercent(), "%"), stockCellWidth, stockCellHeight, stockCellWidth*i + 10, 0, colorTag);
				cell->addChild(card);
				break;
			}
			case 5:
			{

				StockCellCard* card = StockCellCard::createCardSprite(String::createWithFormat("%d", stockVector.at(idx)->getStoreNumber()), stockCellWidth, stockCellHeight, stockCellWidth*i + 10, 0, colorTag);
				cell->addChild(card);
				break;
			}
			}


		}

	}
	else{
		label = (LabelTTF*)cell->getChildByTag(2);
	}

	return cell;
}
ssize_t LineChart::numberOfCellsInTableView(cocos2d::extension::TableView *table){ //大小
	return 5;
}


void LineChart::drawpic()  //画曲线
{
	drawNode->clear();
	int maxValue = getMaxValue(pointvec);
	int maxValue2 = int((maxValue + 100) / 100) * 100;
	maxValue1 = maxValue2 / 10;


	spaceRatio = 0.08f;  //y轴间距系数
	leftRatioX = 0.1f;   //x轴左侧间距系数


	int fontSize = 20;
	string fontName = StringUtils::format("Thonburi");

	Size layerSize = Size(VisibleRect::getVisibleRect().size.width, VisibleRect::getVisibleRect().size.height * 1 / 2);


	layerHeight1 = 30;
	float layerHeight = layerHeight1;
	float layerWidth = layerSize.width;
	int count = layerSize.width / 50;
	/***********************画xy轴标签*************************************/

	for (int i = 0; i < 11; i++) {
		Point bPoint = Point(layerWidth* leftRatioX, layerHeight);
		Point ePoint = Point(layerWidth* leftRatioX + (count - 2) * 50, layerHeight);
		Label* label = Label::createWithSystemFont(StringUtils::format("%d", maxValue1* i).c_str(), fontName.c_str(), fontSize);
		label->setPosition(Point(layerWidth* 0.05f, layerHeight));
		label->setTag(100 + i);
		addChild(label);
		drawNode->drawSegment(bPoint, ePoint, 0.5, Color4F(100, 100, 200, 200));
		layerHeight += layerSize.height * spaceRatio;
	}


	float layer_wd = layerSize.width * leftRatioX;
	for (int i = 0; i < count; i++) {


		Point bPoint = Point(layer_wd, layerHeight1);
		Point ePoint = Point(layer_wd, layerSize.height * spaceRatio * 10 + layerHeight1);
		if (i % 2 == 0)
		{
			drawNode->drawSegment(bPoint, ePoint, 0.5, Color4F(100, 100, 200, 200));
		}

		auto labelX = Label::createWithSystemFont(StringUtils::format("%d", i).c_str(), "Thonburi", 20);
		labelX->setPosition(Point(ePoint.x, 0));
		labelX->setAnchorPoint(ccp(0, 0));
		labelX->setTag(100 + 11 + i);
		this->addChild(labelX);
		layer_wd += 50;

	}


	drawLine(pointvec, Color4B(0, 255, 255, 255), Color4B(255, 0, 0, 255));	//画线
}


void LineChart::drawLine(vector<Point> vec, Color4B lineColor, Color4B dotColor)	//画线
{

	Size layerSize = Size(VisibleRect::getVisibleRect().size.width, VisibleRect::getVisibleRect().size.height * 1 / 2);

	float layerWidth = layerSize.width;

	float tempWidth = layerSize.height * spaceRatio;
	float tempWidth2 = 0;

	float tempHeight1 = maxValue1;

	double  ratio = tempWidth / tempHeight1;


	/**********************开始画线**********************/
	std::vector<Point>::iterator beforePoint;
	std::vector<Point>::iterator currentPoint;

	beforePoint = vec.begin();

	for (currentPoint = vec.begin() + 1; currentPoint != vec.end(); currentPoint++) {
		Point bPoint = *beforePoint;
		bPoint = Point(bPoint.x + layerWidth* leftRatioX, bPoint.y * ratio + layerHeight1 + tempWidth2);

		Point ePoint = *currentPoint;
		ePoint = Point(ePoint.x + layerWidth* leftRatioX, ePoint.y * ratio + layerHeight1 + tempWidth2);

		drawNode->drawSegment(bPoint, ePoint, 0.8, Color4F::RED);
		beforePoint = currentPoint;

	}

	/**********************结束画线**********************/

	/********************开始画点**********************************************/
	beforePoint = vec.begin();
	DrawPrimitives::setDrawColor4B(dotColor.r, dotColor.g, dotColor.b, dotColor.a);
	Point bPoint = *beforePoint;
	bPoint = Point(bPoint.x + layerWidth* leftRatioX, bPoint.y * ratio + layerHeight1 + tempWidth2);

	drawNode->drawDot(bPoint, 5, Color4F::YELLOW);

	int i = 2;
	for (currentPoint = vec.begin() + 1; currentPoint != vec.end(); currentPoint++) {
		Point ePoint = *currentPoint;
		ePoint = Point(ePoint.x + layerWidth* leftRatioX, ePoint.y * ratio + layerHeight1 + tempWidth2);
		drawNode->drawDot(ePoint, 5, Color4F::YELLOW);
		i++;
	}
	/********************结束画点*********************************************END**/

}



void LineChart::setData(vector<float> data)		//设置数据
{
	pointvec.clear();
	vector<float>::iterator it;
	int i = 0;

	for (it = data.begin(); it != data.end(); it++) {
		float f = *it;
		pointvec.push_back(Point(50 * (i + 1), f));
		i++;

	}

}

double LineChart::getMaxValue(std::vector<Point> vec)	//获取最大值
{

	double maxY = 1;

	for (int i = 0; i < vec.size(); i++)
	{
		float num = vec.at(i).y;
		if (maxY < abs(num))
		{
			maxY = abs(num);
		}
	}
	return maxY;
}

