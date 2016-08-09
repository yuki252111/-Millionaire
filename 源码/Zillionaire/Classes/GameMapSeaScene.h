#ifndef __GAME_MAP_SEA_SCENE_H__
#define __GAME_MAP_SEA_SCENE_H__

#include "GameMapBaseScene.h"

USING_NS_CC;
USING_NS_CC_EXT;
//º£ÑóµØÍ¼
class GameMapSeaScene : public GameMapBaseScene
{
public:
	static Scene* createScene();

	virtual bool init();
	virtual void initTiledGrid();
	virtual void initPropTiledID();
	virtual void addMap();
	virtual void addRightBanner();

	CREATE_FUNC(GameMapSeaScene);
	
};

#endif