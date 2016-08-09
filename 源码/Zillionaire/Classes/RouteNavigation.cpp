#include "RouteNavigation.h"
#include "GameMapBaseScene.h"

RouteNavigation* RouteNavigation::routeNavInstance = NULL;

RouteNavigation::RouteNavigation(void)	//构造函数
{
	struct timeval now;
	gettimeofday(&now, NULL);
	unsigned rand_seed = (unsigned)(now.tv_sec * 1000 + now.tv_usec / 1000);    //都转化为毫秒 
	srand(rand_seed);
}


RouteNavigation::~RouteNavigation(void)	//析构函数，释放内存
{
	routeNavInstance = NULL;

	pathRows.clear();
	pathCols.clear();
	std::vector<int>(pathCols).swap(pathCols);
	std::vector<int>(pathRows).swap(pathRows);
}

RouteNavigation* RouteNavigation::getInstance()
{
	if (!routeNavInstance)
	{
		routeNavInstance = new RouteNavigation();
	}
	return routeNavInstance;

}
//找路径
void RouteNavigation::findPath(Player* player, int stepsCount, bool** canPassGrid, int gridRowsCount, int gridColsCount)
{

	pathCols.clear();
	pathRows.clear();

	int nextCol, nextRow;
	int currentCol, currentRow;

	float x = player->getPositionX();
	float y = player->getPositionY();

	currentCol = x / tiledHeight;
	currentRow = (y - tiledWidth) / tiledWidth;

	pathCols.push_back(currentCol);
	pathRows.push_back(currentRow);


	bool** canPassGridCopy = new  bool*[gridRowsCount];
	for (int i = 0; i<gridRowsCount; i++)
		canPassGridCopy[i] = new bool[gridColsCount];



	for (int row = 0; row<gridRowsCount; row++)
	{
		for (int col = 0; col<gridColsCount; col++)
		{
			canPassGridCopy[row][col] = canPassGrid[row][col];
		}
	}

	int rowtemp = player->getComeFromeRow();
	int coltemp = player->getComeFromCol();
	if (rowtemp <= -1 || coltemp <= -1)
	{
		player->setComeFromCol(currentCol);
		player->setComeFromeRow(currentRow);
	}

	canPassGridCopy[player->getComeFromeRow()][player->getComeFromCol()] = false;

	std::vector<bool> directionFour;
	std::vector<int> canPassDirVector;

	int hasGoneNumber = 0;

	while (hasGoneNumber<stepsCount)
	{
		directionFour.clear();
		for (int i = 0; i<4; i++)
		{
			directionFour.push_back(false);
		}
		canPassDirVector.clear();
		//判断上下左右是否可以走
		directionFour[GO_UP] = isCanGoByColRow(currentRow, currentCol, GO_UP, canPassGridCopy);
		directionFour[GO_DOWN] = isCanGoByColRow(currentRow, currentCol, GO_DOWN, canPassGridCopy);
		directionFour[GO_LEFT] = isCanGoByColRow(currentRow, currentCol, GO_LEFT, canPassGridCopy);
		directionFour[GO_RIGHT] = isCanGoByColRow(currentRow, currentCol, GO_RIGHT, canPassGridCopy);

		for (int i = 0; i<4; i++)
		{
			if (directionFour[i])
			{
				canPassDirVector.push_back(i);
			}
		}



		int _rand = 0;
		if (canPassDirVector.size() != 0)
			_rand = rand() % canPassDirVector.size();
		else
			return;
		//随机选个方向
		switch (canPassDirVector[_rand])
		{
		case GO_UP:
		{
			nextRow = currentRow - 1;
			nextCol = currentCol;
			break;
		}


		case GO_DOWN:
		{
			nextRow = currentRow + 1;
			nextCol = currentCol;
			break;
		}
		case GO_LEFT:
		{
			nextRow = currentRow;
			nextCol = currentCol - 1;
			break;
		}
		case GO_RIGHT:
		{
			nextRow = currentRow;
			nextCol = currentCol + 1;
			break;
		}
		}

		pathCols.push_back(nextCol);
		pathRows.push_back(nextRow);

		canPassGridCopy[currentRow][currentCol] = false;

		currentCol = nextCol;
		currentRow = nextRow;

		hasGoneNumber++;

	}
	player->setComeFromCol(pathCols[pathCols.size() - 2]);//设置那里来的，防止走回去
	player->setComeFromeRow(pathRows[pathRows.size() - 2]);

	CC_SAFE_DELETE(canPassGridCopy);
	directionFour.clear();
	canPassDirVector.clear();
	std::vector<bool>(directionFour).swap(directionFour);
	std::vector<int>(canPassDirVector).swap(canPassDirVector);
}

bool RouteNavigation::isCanGoByColRow(int row, int col, int direction, bool** canPassGrid)//判断是否可以走
{
	switch (direction)

	{
	case GO_UP:
	{
		return canPassGrid[row - 1][col];
	}


	case GO_DOWN:
	{
		return canPassGrid[row + 1][col];
	}
	case GO_LEFT:
	{
		return canPassGrid[row][col - 1];
	}
	case GO_RIGHT:
	{
		return canPassGrid[row][col + 1];
	}
	}

	return false;

}