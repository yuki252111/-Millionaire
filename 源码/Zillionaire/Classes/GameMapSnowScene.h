#ifndef __GAME_MAP_SNOW_SCENE_H__
#define __GAME_MAP_SNOW_SCENE_H__

#include "GameMapBaseScene.h"

USING_NS_CC;
USING_NS_CC_EXT;
//Ñ©µØµØÍ¼
class GameMapSnowScene : public GameMapBaseScene
{
public:
	static Scene* createScene();

	virtual bool init();
	virtual void initTiledGrid();
	virtual void initPropTiledID();
	virtual void addMap();
	virtual void addRightBanner();
	CREATE_FUNC(GameMapSnowScene);

};

#endif