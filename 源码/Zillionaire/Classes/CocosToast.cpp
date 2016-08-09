#include "CocosToast.h"
//创建吐司动画
void CocosToast::createToast(cocos2d::Node *node, const std::string &msg, const float &time, Point point)
{
	auto label = Label::createWithSystemFont(msg.c_str(), "Arial", 20); //文字颜色
	label->setColor(Color3B(255,255,0));
	label->ignoreAnchorPointForPosition(false);
	label->setAnchorPoint(Point::ANCHOR_MIDDLE);

	auto layer = LayerColor::create(Color4B(100, 100, 100, 255)); //背景颜色
	layer->ignoreAnchorPointForPosition(false);
	layer->setAnchorPoint(Point::ANCHOR_MIDDLE);
	layer->setContentSize(label->getContentSize() + Size(20, 15));

	node->addChild(layer);
	node->addChild(label);
	layer->setPosition(point);
	label->setPosition(layer->getPosition());
	auto seq1 = Sequence::create(FadeIn::create(time / 5), DelayTime::create(time / 5 * 1.5), FadeOut::create(time / 5 * 2.5), CallFuncN::create(layer, callfuncN_selector(CocosToast::removeToast)), NULL);
	auto seq2 = Sequence::create(EaseSineIn::create(MoveBy::create(time / 5, Point(0, 50))), DelayTime::create(time / 5 * 2), EaseSineOut::create(MoveBy::create(time / 3, Point(0, -50))), NULL);
	auto spawn = Spawn::create(seq1, seq2, NULL);
	auto action = Repeat::create(spawn, 1);
	layer->setOpacity(0);
	label->setOpacity(0);
	layer->runAction(action);
	label->runAction(action->clone());
}

void CocosToast::removeToast(Node* node)
{
	this->removeFromParentAndCleanup(true);

}
