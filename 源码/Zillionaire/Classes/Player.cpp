#include "Player.h"
#include "GameController.h"
#include "GameMapBaseScene.h"
#include "Stock.h"

Player::Player()
{
	_comeFromeRow = -1;   //来源，防止从原来的路返回
	_comeFromCol = -1;
	restDay = 0;
}
Player::~Player()
{
	playerSpriteFrameCache->release();
	left->release();
	right->release();
	up->release();
	down->release();
}
void Player::lotteryClear()	//清除彩票
{
	lotteryVector.clear();
}
void Player::addLottery(int num)  //添加彩票
{
	lotteryVector.push_back(num);
}
void Player::addShopItem(int num)  //添加道具
{
	std::map<int, int>::iterator iter = shopItemMap.begin();
	bool found = false;
	while (iter != shopItemMap.end())
	{
		if (iter->first == num)
		{
			iter->second = iter->second + 1;
			found = true;
			break;
		}
		iter++;
	}
	if (found == false)
		shopItemMap[num] = 1;
}

Player* Player::createPlayer(char* name, int tag, int money, int strength) //创建角色
{
	Player* player = new Player();
	player->init(name, tag, money, strength);
	player->autorelease();
	return player;
}


bool Player::init(char* name, int tag, int money, int strength)
{

	this->setTag(tag);
	addPlayerAnimation();  //角色动画
	SpriteFrame* spf;
	switch (tag){
	case PLAYER_1_TAG:
	{	
		spf = playerSpriteFrameCache->getSpriteFrameByName("player1_anim_01.png");
		break;
	}
	case PLAYER_2_TAG:
	{
		spf = playerSpriteFrameCache->getSpriteFrameByName("player2_anim_02.png");
		break;
	}
	}

	setPlayerAnimate();
	Sprite::initWithSpriteFrame(spf);  //设置当前精灵
	_name = name;
	_money = money;
	_strength = strength;
	//添加股票
	stockMap.insert(0, Stock::create(800100, String::create("rich_technology"), 0, 100, 0, 100));
	stockMap.insert(1, Stock::create(800200, String::create("rich_technology"), 0, 200, 0, 100));
	stockMap.insert(2, Stock::create(800300, String::create("rich_technology"), 0, 70, 0, 0));
	stockMap.insert(3, Stock::create(800400, String::create("rich_technology"), 0, 100, 0, 100));
	stockMap.insert(4, Stock::create(800500, String::create("rich_technology"), 0, 150, 0, 0));


	return true;
}
void Player::addPlayerAnimation()          //添加动画
{
	playerSpriteFrameCache = SpriteFrameCache::getInstance();

	int tag = getTag();
	switch (tag)
	{
	case PLAYER_1_TAG:
	{
		playerSpriteFrameCache->addSpriteFramesWithFile(PLAYER1_ANIM_PLIST, PLAYER1_ANIM_PNG);
		break;
	}
	case PLAYER_2_TAG:
	{
		playerSpriteFrameCache->addSpriteFramesWithFile(PLAYER2_ANIM_PLIST, PLAYER2_ANIM_PNG);
		break;
	}
	}
	playerSpriteFrameCache->retain();

	char name[20];
	memset(name, 0, 20);

	for (int i = 1; i <= 4; i++)
	{
		sprintf(name, "player%d_anim_%02d.png", tag, i);
		player_anim_left_vector.pushBack(playerSpriteFrameCache->getSpriteFrameByName(name));

	}
	for (int i = 5; i <= 8; i++)
	{
		sprintf(name, "player%d_anim_%02d.png", tag, i);
		player_anim_right_vector.pushBack(playerSpriteFrameCache->getSpriteFrameByName(name));


	}
	for (int i = 9; i <= 12; i++)
	{
		sprintf(name, "player%d_anim_%02d.png", tag, i);
		player_anim_down_vector.pushBack(playerSpriteFrameCache->getSpriteFrameByName(name));

	}
	for (int i = 13; i <= 16; i++)
	{
		sprintf(name, "player%d_anim_%02d.png", tag, i);
		player_anim_up_vector.pushBack(playerSpriteFrameCache->getSpriteFrameByName(name));

	}
}
void Player::setPlayerAnimate()  //设置动画
{
	int tag = getTag();
	char left_animation[20];
	memset(left_animation, 0, 20);
	sprintf(left_animation, "left_animation_%d", tag);

	char right_animation[20];
	memset(right_animation, 0, 20);
	sprintf(right_animation, "right_animation_%d", tag);

	char down_animation[20];
	memset(down_animation, 0, 20);
	sprintf(down_animation, "down_animation_%d", tag);

	char up_animation[20];
	memset(up_animation, 0, 20);
	sprintf(up_animation, "up_animation_%d", tag);

	if (!AnimationCache::getInstance()->getAnimation(left_animation))
	{
		AnimationCache::getInstance()->addAnimation(Animation::createWithSpriteFrames(player_anim_left_vector, playerGoPerFrameTime), left_animation);
	}
	if (!AnimationCache::getInstance()->getAnimation(right_animation))
	{
		AnimationCache::getInstance()->addAnimation(Animation::createWithSpriteFrames(player_anim_right_vector, playerGoPerFrameTime), right_animation);
	}
	if (!AnimationCache::getInstance()->getAnimation(down_animation))
	{
		AnimationCache::getInstance()->addAnimation(Animation::createWithSpriteFrames(player_anim_down_vector, playerGoPerFrameTime), down_animation);
	}
	if (!AnimationCache::getInstance()->getAnimation(up_animation))
	{
		AnimationCache::getInstance()->addAnimation(Animation::createWithSpriteFrames(player_anim_up_vector, playerGoPerFrameTime), up_animation);
	}
	//创建上下左右动画
	left = Animate::create(AnimationCache::getInstance()->getAnimation(left_animation)); 
	right = Animate::create(AnimationCache::getInstance()->getAnimation(right_animation)); 
	down = Animate::create(AnimationCache::getInstance()->getAnimation(down_animation));
	up = Animate::create(AnimationCache::getInstance()->getAnimation(up_animation));
	left->retain();
	right->retain();
	down->retain();
	up->retain();
}
void Player::startGo(std::vector<int> rowVector, std::vector<int> colVector) //开始走
{
	GameMapBaseScene::drawPathMark(rowVector, colVector);//路标

	GameController* gameController = GameController::getInstance();
	gameController->startGo(rowVector, colVector, this); //开始走
}
