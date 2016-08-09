#ifndef __LINE_CHART_H__
#define __LINE_CHART_H__

#include "cocos2d.h"
#include "VisibleRect.h"
#include "StockCellCard.h"
#include "Stock.h"
#include "Udefine.h"
#include "UFunction.h"
#include <cocos-ext.h>
#include "CocosToast.h"
#include "Player.h"
#include "editor-support\cocostudio\CCSGUIReader.h"
#include "ui\CocosGUI.h"

using namespace cocos2d::ui;
using namespace cocostudio;
using namespace std;
USING_NS_CC;
USING_NS_CC_EXT;


//画股票界面
class LineChart :public Layer, TableViewDataSource, TableViewDelegate {

public:
	static LineChart* createChart(Player* player);

private:

	Player* richerPlayer;
	int selectedTag;
	DrawNode* drawNode;
	TableView * tv;
	Sprite* bgSprite;


	Vector<Stock*> stockVector;
	Map<int, Stock*> playerStockMap;
	void initStockVector(Map<int, Stock*> stockMap);


	bool initChart(Player* player);
	CC_SYNTHESIZE(int, _hInterval, HInterval);
	CC_SYNTHESIZE(int, _vInterval, VInterval);
	Vector<float>* firstVec;

	void drawpic();
	double  getMaxValue(std::vector<Point>  vec);
	std::vector<Point> pointvec;
	void drawLine(vector<Point> vec, Color4B lineColor, Color4B dotColor);
	void setData(vector<float> data);
	float spaceRatio;  //y轴间距系数
	float leftRatioX;   //x轴左侧间距系数
	int maxValue1;    //数据中的最大值
	float  layerHeight1;  //图离底部的距离

	virtual Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view){};
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view){};

	Sprite* arrowSprite_left;
	Sprite* arrowSprite_right;
	void initMenu();
	void buttonCallback(CCObject* pSender);
	Menu* _menu;

	int moveTag;
	vector<float> getsock_pointVec(int id);
	Label* playerMoneyLabel;

};

#endif 
