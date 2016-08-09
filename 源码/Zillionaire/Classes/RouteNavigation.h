#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "cocos2d.h"
#include "Player.h"

USING_NS_CC;


const int GO_UP = 0;
const int GO_DOWN = 1;
const int GO_LEFT = 2;
const int GO_RIGHT = 3;
//单例类,找路径
class RouteNavigation{
public:
	static RouteNavigation* routeNavInstance;
	static RouteNavigation* getInstance();
	void findPath(Player* player, int stepsCount, bool** canPassGrid, int gridRowsCount, int gridColsCount);//寻找路径
	
	RouteNavigation();
	~RouteNavigation();
private:
	CC_SYNTHESIZE(std::vector<int>, pathCols, PathCols);	//路径行
	CC_SYNTHESIZE(std::vector<int>, pathRows, PathRows);	//路径列

	bool isCanGoByColRow(int row, int col, int direction, bool** canPassGrid);

};

#endif