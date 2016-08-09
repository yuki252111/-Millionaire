#ifndef __VISIBLE_RECT_H__
#define __VISIBLE_RECT_H__

#include "cocos2d.h"
USING_NS_CC;
//一些坐标
class VisibleRect
{
public:
	static Rect getVisibleRect();	//长方形

	static Point left();	//左
	static Point right();	//右
	static Point top();		//上
	static Point bottom();	//下
	static Point center();	//中
	static Point leftTop();	//左上
	static Point rightTop();//右上
	static Point leftBottom();//左下
	static Point rightBottom();//右下
private:
	static void lazyInit();
	static Rect s_visibleRect;
};


#endif 
