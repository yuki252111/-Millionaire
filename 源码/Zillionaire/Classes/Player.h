#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "UDefine.h"
#include "ODSocket.h"
#include "Stock.h"
#include <map>

USING_NS_CC;
USING_NS_CC_EXT;

class Player : public Sprite
{
public:
	Player();
	~Player();

	static Player* createPlayer(char* name, int tag,int money = 20000, int strength = 200); //�������
	void startGo(std::vector<int> rowVector, std::vector<int> colVector);             
	//����֡
	CC_SYNTHESIZE(Vector<SpriteFrame*>, player_anim_left_vector, Anim_left_vector);
	CC_SYNTHESIZE(Vector<SpriteFrame*>, player_anim_right_vector, Anim_right_vector);
	CC_SYNTHESIZE(Vector<SpriteFrame*>, player_anim_down_vector, Anim_down_vector);
	CC_SYNTHESIZE(Vector<SpriteFrame*>, player_anim_up_vector, Anim_up_vector);
	//����
	CC_SYNTHESIZE(Animate*, left, Left);
	CC_SYNTHESIZE(Animate*, right, Right);
	CC_SYNTHESIZE(Animate*, down, Down);
	CC_SYNTHESIZE(Animate*, up, Up);
	//��Ʊ
	CC_SYNTHESIZE(std::vector<int>, lotteryVector, LotteryVector);
	CC_SYNTHESIZE(SpriteFrameCache*, playerSpriteFrameCache, PlayerSpriteFrameCache);
	//��Ʊ
	void addShopItem(int num);
	std::map<int, int>shopItemMap;

	void addPlayerAnimation();
	void setPlayerAnimate();
	void addLottery(int num);
	void lotteryClear();
	Map<int, Stock*> stockMap;
	bool isUsed;
	int restDay;       //��Ϣ����

private:
	CC_SYNTHESIZE(std::string, _name, Name);
	CC_SYNTHESIZE(int, _money, Money);
	CC_SYNTHESIZE(int, _strength, Strength);
	CC_SYNTHESIZE(int, _comeFromeRow, ComeFromeRow);
	CC_SYNTHESIZE(int, _comeFromCol, ComeFromCol);

	bool init(char* name, int tag,int money, int strength);
};

#endif