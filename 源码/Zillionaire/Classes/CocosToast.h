#ifndef __COCOS_TOAST_H__
#define __COCOS_TOAST_H__

#include "cocos2d.h"
#include "cocos-ext.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
//土司类，弹出提示语句
class CocosToast : public LayerColor
{
public:
	static void createToast(Node* node, const std::string& msg, const float& time, Point point);//静态函数，方便类直接调用
	void removeToast(Node* node);
};

#endif