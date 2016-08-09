#ifndef __COCOS_TOAST_H__
#define __COCOS_TOAST_H__

#include "cocos2d.h"
#include "cocos-ext.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
//��˾�࣬������ʾ���
class CocosToast : public LayerColor
{
public:
	static void createToast(Node* node, const std::string& msg, const float& time, Point point);//��̬������������ֱ�ӵ���
	void removeToast(Node* node);
};

#endif