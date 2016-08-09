#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "cocos2d.h"
#include "Player.h"

USING_NS_CC;


const int GO_UP = 0;
const int GO_DOWN = 1;
const int GO_LEFT = 2;
const int GO_RIGHT = 3;
//������,��·��
class RouteNavigation{
public:
	static RouteNavigation* routeNavInstance;
	static RouteNavigation* getInstance();
	void findPath(Player* player, int stepsCount, bool** canPassGrid, int gridRowsCount, int gridColsCount);//Ѱ��·��
	
	RouteNavigation();
	~RouteNavigation();
private:
	CC_SYNTHESIZE(std::vector<int>, pathCols, PathCols);	//·����
	CC_SYNTHESIZE(std::vector<int>, pathRows, PathRows);	//·����

	bool isCanGoByColRow(int row, int col, int direction, bool** canPassGrid);

};

#endif