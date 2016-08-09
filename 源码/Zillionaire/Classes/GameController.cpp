#include "GameController.h"
#include "GameMapBaseScene.h"
#include "Dice.h"

GameController* GameController::gameControllerInstance = NULL;

bool GameController::init()	//初始化
{
	if (!Layer::init())
	{
		return false;
	}

	callEndGoFunc = CallFunc::create(CC_CALLBACK_0(GameController::endGo, this)); //创建回调函数

	positionAroundEnd = new  float*[4];
	for (int i = 0; i<4; i++)
		positionAroundEnd[i] = new float[2];
	registerObserver();		//注册消息
	return true;
}
void GameController::registerObserver()  //注册消息
{
	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(GameController::dealMsg),
		MSG_CONTROLLER,
		NULL);
}
void GameController::dealMsg(Object* data)  //处理消息
{
	int retMsgType = ((String*)data)->intValue();

	if (retMsgType == MSG_CONTROLLER_PASS_TAG)
	{
		moveOneStep(this->player);
	}
	if (retMsgType == MSG_CONTROLLER_END_TAG)
	{
		handlePropEvent();
	}
	if (retMsgType == MSG_CONTROLLER_AROUND_TAG)
	{
		aroundLandEvent();
	}
}
GameController* GameController::getInstance()	//获取单例
{
	if (!gameControllerInstance)
	{
		gameControllerInstance = GameController::create();
	}
	
	gameControllerInstance->retain();
	return gameControllerInstance;

}

GameController::~GameController()
{
	spawnAction->release();
}

void GameController::moveOneStep(Player* player)  //加载行走动画
{
	nextRow = rowVector[stepHasGone + 1];
	nextCol = colVector[stepHasGone + 1];
	int distanceRow = nextRow - currentRow;
	int distanceCol = nextCol - currentCol;
	//判断行走方向
	if (distanceRow >0)//up
	{
		moveBy = MoveBy::create(playerGoTotalTime, ccp(0, tiledHeight));
		repeate = Repeat::create(player->getUp(), 1);
	}
	if (distanceRow <0)//down
	{
		moveBy = MoveBy::create(playerGoTotalTime, ccp(0, -tiledHeight));
		repeate = Repeat::create(player->getDown(), 1);
	}
	if (distanceCol >0)//right
	{
		moveBy = MoveBy::create(playerGoTotalTime, ccp(tiledWidth, 0));
		repeate = Repeat::create(player->getRight(), 1);
	}
	if (distanceCol <0)//left
	{
		moveBy = MoveBy::create(playerGoTotalTime, ccp(-tiledWidth, 0));
		repeate = Repeat::create(player->getLeft(), 1);
	}

	spawnAction = Sequence::create(Spawn::create(moveBy, repeate, NULL), callEndGoFunc, NULL);
	spawnAction->retain();
	player->runAction(spawnAction);
}

void GameController::startGo(std::vector<int> rowVector, std::vector<int> colVector, Player* player)	//开始走
{
	this->currentRow = rowVector[0];
	this->currentCol = colVector[0];
	this->nextRow = 0;
	this->nextCol = 0;

	this->rowVector = rowVector;
	this->colVector = colVector;
	this->player = player;
	this->stepHasGone = 0;
	this->stepsCount = rowVector.size() - 1;
	moveOneStep(player);

}

void GameController::endGo()  //结束一步

{
	GameMapBaseScene::pathMarkVector.at(stepHasGone)->setVisible(false);
	stepHasGone++;

	if (stepHasGone >= stepsCount)
	{
		String * str = String::createWithFormat("%d", MSG_BUY_NOTHING_TAG);
		NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		return;
	}
	currentRow = nextRow;
	currentCol = nextCol;

	float playerEnd_X = colVector[stepHasGone] * 32;
	float playerEnd_Y = rowVector[stepHasGone] * 32 + 32;
	//判断是否经过道具
	Point pointInMap = UFunction::GL2map(Point(playerEnd_X, playerEnd_Y), GameMapBaseScene::_map);
	int endId = GameMapBaseScene::wayLayer->getTileGIDAt(pointInMap);
	if (endId == GameMapBaseScene::lottery_tiledID)	//彩票
	{
		String * str = String::createWithFormat("%d-%f-%f-%d-%d", MSG_BUY_LOTTERY_TAG, pointInMap.x, pointInMap.y, player->getTag(),MSG_GO_PASS_TAG);
		NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		return;
	}
	if (endId == GameMapBaseScene::stock_tiledID)	//股票
	{
		String * str = String::createWithFormat("%d-%f-%f-%d-%d", MSG_BUY_STOCK_TAG, 1.0f, 1.0f, player->getTag(),MSG_GO_PASS_TAG);
		NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		return;
	}
	if (pointInMap == GameMapBaseScene::shopPoint)	//道具
	{
		String * str = String::createWithFormat("%d-%f-%f-%d-%d", MSG_BUY_SHOP_ITEM_TAG, pointInMap.x, pointInMap.y, player->getTag(), MSG_GO_PASS_TAG);
		NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		return;
	}
	moveOneStep(this->player);
}
void GameController::handlePropEvent()
{
	//判断是否停在道具
	//------------------------------------------------
	float playerEnd_X = colVector[stepsCount] * 32;
	float playerEnd_Y = rowVector[stepsCount] * 32 + 32;

	Point pointInMap = UFunction::GL2map(Point(playerEnd_X, playerEnd_Y), GameMapBaseScene::_map);
	int endId = GameMapBaseScene::wayLayer->getTileGIDAt(pointInMap);
	if (endId == GameMapBaseScene::randomEvent_tiledID)	//随机事件
	{
		String * str = String::createWithFormat("%d-%f-%f-%d-%d", MSG_RANDOM_ASK_EVENT_TAG, pointInMap.x, pointInMap.y, player->getTag(), MSG_GO_END_TAG);
		NotificationCenter::getInstance()->postNotification(MSG_RANDOM_ASK_EVENT, str);
		return;
	}
	if (endId == GameMapBaseScene::strength_30_tiledID)	//体力回升
	{
		String * str = String::createWithFormat("%d-%f-%f-%d-%d", MSG_STRENGTH_UP30_TAG, pointInMap.x, pointInMap.y, player->getTag(), MSG_GO_END_TAG);
		NotificationCenter::getInstance()->postNotification(MSG_STRENGTH, str);
		return;
	}
	if (endId == GameMapBaseScene::strength_50_tiledID)	//体力回升
	{
		String * str = String::createWithFormat("%d-%f-%f-%d-%d", MSG_STRENGTH_UP50_TAG, pointInMap.x, pointInMap.y, player->getTag(), MSG_GO_END_TAG);
		NotificationCenter::getInstance()->postNotification(MSG_STRENGTH, str);
		return;
	}
	if (endId == GameMapBaseScene::strength_80_tiledID)	//体力回升
	{
		String * str = String::createWithFormat("%d-%f-%f-%d-%d", MSG_STRENGTH_UP80_TAG, pointInMap.x, pointInMap.y, player->getTag(),MSG_GO_END_TAG);
		NotificationCenter::getInstance()->postNotification(MSG_STRENGTH, str);
		return;
	}
	if (endId == GameMapBaseScene::lottery_tiledID)	//彩票
	{
		String * str = String::createWithFormat("%d-%f-%f-%d-%d", MSG_BUY_LOTTERY_TAG, pointInMap.x, pointInMap.y, player->getTag(),MSG_GO_END_TAG);
		NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		return;
	}
	if (endId == GameMapBaseScene::stock_tiledID)	//股票
	{
		String * str = String::createWithFormat("%d-%f-%f-%d-%d", MSG_BUY_STOCK_TAG, 1.0f, 1.0f, player->getTag(),MSG_GO_END_TAG);
		NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		return;
	}
	if (pointInMap == GameMapBaseScene::shopPoint)	//商店
	{
		String * str = String::createWithFormat("%d-%f-%f-%d-%d", MSG_BUY_SHOP_ITEM_TAG, pointInMap.x, pointInMap.y, player->getTag(), MSG_GO_END_TAG);
		NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
		return;
	}
	if (pointInMap == GameMapBaseScene::godPoint1 ||
		pointInMap == GameMapBaseScene::godPoint2)	//土地公，土地婆
	{
		positionAroundEnd[0][0] = playerEnd_X;
		positionAroundEnd[0][1] = playerEnd_Y + 32;

		// down
		positionAroundEnd[1][0] = playerEnd_X;
		positionAroundEnd[1][1] = playerEnd_Y - 32;

		// left
		positionAroundEnd[2][0] = playerEnd_X - 32;
		positionAroundEnd[2][1] = playerEnd_Y;

		// right
		positionAroundEnd[3][0] = playerEnd_X + 32;
		positionAroundEnd[3][1] = playerEnd_Y;

		for (int i = 0; i < 4; i++)	//强占土地
		{
			Point ptMap = UFunction::GL2map(Point(positionAroundEnd[i][0], positionAroundEnd[i][1]), GameMapBaseScene::_map);
			int titleId = GameMapBaseScene::_land->getTileGIDAt(ptMap);

			if (player->getTag() == PLAYER_1_TAG&&(titleId==GameMapBaseScene::blank_land_tiledID||
				titleId==GameMapBaseScene::player2_building_1_tiledID||titleId==GameMapBaseScene::player2_building_2_tiledID||
				titleId==GameMapBaseScene::player2_building_3_tiledID))
			{
				String * str = String::createWithFormat("%d-%f-%f-%d-%d", MSG_BUY_GOD_TAG, ptMap.x, ptMap.y, player->getTag(), titleId);
				NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
				return;
			}
			else if (player->getTag() == PLAYER_2_TAG && (titleId == GameMapBaseScene::blank_land_tiledID ||
				titleId == GameMapBaseScene::player1_building_1_tiledID || titleId == GameMapBaseScene::player1_building_2_tiledID ||
				titleId == GameMapBaseScene::player1_building_3_tiledID))
			{
				String * str = String::createWithFormat("%d-%f-%f-%d-%d", MSG_BUY_GOD_TAG, ptMap.x, ptMap.y, player->getTag(), titleId);
				NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
				return;
			}
		}
	}
	if (pointInMap == GameMapBaseScene::trapPoint)	//陷阱
	{
		String * str = String::createWithFormat("%d-%f-%f-%d", MSG_GO_TRAP_TAG, pointInMap.x, pointInMap.y, player->getTag());
		NotificationCenter::getInstance()->postNotification(MSG_GO, str);
		return;
	}
	NotificationCenter::getInstance()->postNotification(MSG_CONTROLLER,String::createWithFormat("%d",MSG_CONTROLLER_AROUND_TAG));
}
void GameController::aroundLandEvent()
{
	//判断是否有土地
	//-------------------------------------------------
	float playerEnd_X = colVector[stepsCount] * 32;
	float playerEnd_Y = rowVector[stepsCount] * 32 + 32;
	// up
	positionAroundEnd[0][0] = playerEnd_X;
	positionAroundEnd[0][1] = playerEnd_Y + 32;

	// down
	positionAroundEnd[1][0] = playerEnd_X;
	positionAroundEnd[1][1] = playerEnd_Y - 32;

	// left
	positionAroundEnd[2][0] = playerEnd_X - 32;
	positionAroundEnd[2][1] = playerEnd_Y;

	// right
	positionAroundEnd[3][0] = playerEnd_X + 32;
	positionAroundEnd[3][1] = playerEnd_Y;

	bool isEvent = false;
	for (int i = 0; i < 4; i++)
	{
		Point ptMap = UFunction::GL2map(Point(positionAroundEnd[i][0], positionAroundEnd[i][1]), GameMapBaseScene::_map);
		int titleId = GameMapBaseScene::_land->getTileGIDAt(ptMap);
		float x = ptMap.x;
		float y = ptMap.y;

		if (titleId == GameMapBaseScene::blank_land_tiledID)	//空地
		{
			isEvent = true;
			String * str = String::createWithFormat("%d-%f-%f-%d", MSG_BUY_BLANK_DIALOG, x, y, player->getTag());
			NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
			break;
		}
		if (titleId == GameMapBaseScene::player1_building_1_tiledID)	//player1土地
		{
			isEvent = true;
			if (player->getTag() == PLAYER_1_TAG)
			{
				String * str = String::createWithFormat("%d-%f-%f-%d", MSG_BUY_LAND_1_DIALOG, x, y, player->getTag());
				NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
			}
			else
			{
				String * str = String::createWithFormat("%d-%f-%f-%d", MSG_PAY_TOLLS_1_TAG, x, y, player->getTag());
				NotificationCenter::getInstance()->postNotification(MSG_PAY_TOLLS, str);
			}

			break;
		}
		if (titleId == GameMapBaseScene::player1_building_2_tiledID)	//player1土地
		{
			isEvent = true;
			if (player->getTag() == PLAYER_1_TAG)
			{
				String * str = String::createWithFormat("%d-%f-%f-%d", MSG_BUY_LAND_2_DIALOG, x, y, player->getTag());
				NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
			}
			else
			{
				String * str = String::createWithFormat("%d-%f-%f-%d", MSG_PAY_TOLLS_2_TAG, x, y, player->getTag());
				NotificationCenter::getInstance()->postNotification(MSG_PAY_TOLLS, str);
			}
			break;
		}

		if (titleId == GameMapBaseScene::player1_building_3_tiledID)	//player1土地
		{
			if (player->getTag() != PLAYER_1_TAG)
			{
				isEvent = true;
				String * str = String::createWithFormat("%d-%f-%f-%d", MSG_PAY_TOLLS_3_TAG, x, y, player->getTag());
				NotificationCenter::getInstance()->postNotification(MSG_PAY_TOLLS, str);
			}
			break;
		}

		if (titleId == GameMapBaseScene::player2_building_1_tiledID)	//player2土地
		{
			isEvent = true;
			if (player->getTag() == PLAYER_2_TAG)
			{
				String * str = String::createWithFormat("%d-%f-%f-%d", MSG_BUY_LAND_1_DIALOG, x, y, player->getTag());
				NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
			}
			else
			{
				String * str = String::createWithFormat("%d-%f-%f-%d", MSG_PAY_TOLLS_1_TAG, x, y, player->getTag());
				NotificationCenter::getInstance()->postNotification(MSG_PAY_TOLLS, str);
			}
			break;
		}
		if (titleId == GameMapBaseScene::player2_building_2_tiledID)	//player2土地
		{
			isEvent = true;
			if (player->getTag() == PLAYER_2_TAG)
			{
				String * str = String::createWithFormat("%d-%f-%f-%d", MSG_BUY_LAND_2_DIALOG, x, y, player->getTag());
				NotificationCenter::getInstance()->postNotification(MSG_BUY, str);
			}
			else
			{
				String * str = String::createWithFormat("%d-%f-%f-%d", MSG_PAY_TOLLS_2_TAG, x, y, player->getTag());
				NotificationCenter::getInstance()->postNotification(MSG_PAY_TOLLS, str);
			}
			break;
		}
		if (titleId == GameMapBaseScene::player2_building_3_tiledID)	//player2土地
		{
			if (player->getTag() != PLAYER_2_TAG)
			{
				isEvent = true;
				String * str = String::createWithFormat("%d-%f-%f-%d", MSG_PAY_TOLLS_3_TAG, x, y, player->getTag());
				NotificationCenter::getInstance()->postNotification(MSG_PAY_TOLLS, str);
			}
			break;
		}
	}
	if (!isEvent)	//没有土地
		NotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUY, String::createWithFormat("%d", MSG_BUY_LOTTERY_PUBLISH_TAG));
}