#include "VisibleRect.h"
USING_NS_CC;

Rect VisibleRect::s_visibleRect;

void VisibleRect::lazyInit()  //整个大小
{
	s_visibleRect = Director::getInstance()->getOpenGLView()->getVisibleRect();
}

Rect VisibleRect::getVisibleRect()	//获取长方形大小
{
	lazyInit();
	return s_visibleRect;
}

Point VisibleRect::left() //左
{
	lazyInit();
	return Point(s_visibleRect.origin.x, s_visibleRect.origin.y + s_visibleRect.size.height / 2);
}

Point VisibleRect::right() //右
{
	lazyInit();
	return Point(s_visibleRect.origin.x + s_visibleRect.size.width, s_visibleRect.origin.y + s_visibleRect.size.height / 2);
}

Point VisibleRect::top() //上
{
	lazyInit();
	return Point(s_visibleRect.origin.x + s_visibleRect.size.width / 2, s_visibleRect.origin.y + s_visibleRect.size.height);
}

Point VisibleRect::bottom() //下
{
	lazyInit();
	return Point(s_visibleRect.origin.x + s_visibleRect.size.width / 2, s_visibleRect.origin.y);
}

Point VisibleRect::center() //中间
{
	lazyInit();
	return Point(s_visibleRect.origin.x + s_visibleRect.size.width / 2, s_visibleRect.origin.y + s_visibleRect.size.height / 2);
}

Point VisibleRect::leftTop() //左上
{
	lazyInit();
	return Point(s_visibleRect.origin.x, s_visibleRect.origin.y + s_visibleRect.size.height);
}

Point VisibleRect::rightTop() //右上
{
	lazyInit();
	return Point(s_visibleRect.origin.x + s_visibleRect.size.width, s_visibleRect.origin.y + s_visibleRect.size.height);
}

Point VisibleRect::leftBottom() //左下
{
	lazyInit();
	return s_visibleRect.origin;
}

Point VisibleRect::rightBottom() //右下
{
	lazyInit();
	return Point(s_visibleRect.origin.x + s_visibleRect.size.width, s_visibleRect.origin.y);
}

