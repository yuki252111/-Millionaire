#ifndef __UFUNCTION_H__
#define __UFUNCTION_H__

#define winSize Director::getInstance()->getWinSize()
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "UDefine.h"
#include "GameMusicScene.h"
USING_NS_CC;
//一些常用函数
struct UFunction
{
	static Point map2GL(const Point& ptMap, TMXTiledMap* map) //地图转GL
	{
		Point ptUI;
		ptUI.x = ptMap.x * map->getTileSize().width;
		ptUI.y = (ptMap.y + 1)* map->getTileSize().height;

		Point ptGL = ptUI;
		ptGL.y = map->getContentSize().height - ptUI.y;
		return ptGL;
	}
	static Point GL2map(const Point& ptGL, TMXTiledMap* map) //GL转地图
	{
		Point ptUI = ptGL;
		ptUI.y = map->getContentSize().height - ptGL.y;

		int x = ptUI.x / map->getTileSize().width;
		int y = ptUI.y / map->getTileSize().height;
		return ccp(x, y);
	}
	//分开字符串
	static void splitString(const char* srcStr, const char* sSep, std::vector<std::string>& messageStr)
	{

		int size = strlen(srcStr);
		std::string str(srcStr);

		int startIndex = 0;
		int endIndex = 0;
		endIndex = str.find(sSep);

		std::string spliStr = "";

		while (endIndex > 0) {
			/* 截取字符串 */
			spliStr = str.substr(startIndex, endIndex-startIndex);
			/* 添加字符串到列表 */
			messageStr.push_back(spliStr);
			/* 截取剩下的字符串 */
			size = str.length();
			str = str.substr(endIndex + 1, size-1-endIndex);
			if (str == "")
				break;
			/* 查找下一个分隔符所在下标 */
			endIndex = 0;
			endIndex = str.find(sSep);
		}
		/* 剩下的字符串也添加进列表 */
		if (str.compare("") != 0) {
			messageStr.push_back(str);
		}
	}
	static void playBgMusic(char* musicName, bool isLoop)	//播放背景音乐
	{
		bool music_on = GameMusicScene::bgOn;

		if (music_on)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(musicName, isLoop);
		}
	}

	static void playEffectMusic(const char* effectName, bool isLoop)	//播放效果音效
	{
		bool music_on = GameMusicScene::effOn;
		
		if (music_on)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(effectName, isLoop);
		}
	}

	static void playEffectMusicRandom(Vector<String*> effectVec, bool isLoop)	//随机效果音效
	{
		bool music_on = GameMusicScene::effOn;
		if (music_on)
		{
			playEffectMusic(effectVec.at(rand() % effectVec.size())->getCString(), isLoop);
		}
	}

	static void stopAudioPlay()	//停止播放
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
	}
};

#endif