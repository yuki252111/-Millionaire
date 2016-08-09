#ifndef __UFUNCTION_H__
#define __UFUNCTION_H__

#define winSize Director::getInstance()->getWinSize()
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "UDefine.h"
#include "GameMusicScene.h"
USING_NS_CC;
//һЩ���ú���
struct UFunction
{
	static Point map2GL(const Point& ptMap, TMXTiledMap* map) //��ͼתGL
	{
		Point ptUI;
		ptUI.x = ptMap.x * map->getTileSize().width;
		ptUI.y = (ptMap.y + 1)* map->getTileSize().height;

		Point ptGL = ptUI;
		ptGL.y = map->getContentSize().height - ptUI.y;
		return ptGL;
	}
	static Point GL2map(const Point& ptGL, TMXTiledMap* map) //GLת��ͼ
	{
		Point ptUI = ptGL;
		ptUI.y = map->getContentSize().height - ptGL.y;

		int x = ptUI.x / map->getTileSize().width;
		int y = ptUI.y / map->getTileSize().height;
		return ccp(x, y);
	}
	//�ֿ��ַ���
	static void splitString(const char* srcStr, const char* sSep, std::vector<std::string>& messageStr)
	{

		int size = strlen(srcStr);
		std::string str(srcStr);

		int startIndex = 0;
		int endIndex = 0;
		endIndex = str.find(sSep);

		std::string spliStr = "";

		while (endIndex > 0) {
			/* ��ȡ�ַ��� */
			spliStr = str.substr(startIndex, endIndex-startIndex);
			/* ����ַ������б� */
			messageStr.push_back(spliStr);
			/* ��ȡʣ�µ��ַ��� */
			size = str.length();
			str = str.substr(endIndex + 1, size-1-endIndex);
			if (str == "")
				break;
			/* ������һ���ָ��������±� */
			endIndex = 0;
			endIndex = str.find(sSep);
		}
		/* ʣ�µ��ַ���Ҳ��ӽ��б� */
		if (str.compare("") != 0) {
			messageStr.push_back(str);
		}
	}
	static void playBgMusic(char* musicName, bool isLoop)	//���ű�������
	{
		bool music_on = GameMusicScene::bgOn;

		if (music_on)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(musicName, isLoop);
		}
	}

	static void playEffectMusic(const char* effectName, bool isLoop)	//����Ч����Ч
	{
		bool music_on = GameMusicScene::effOn;
		
		if (music_on)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(effectName, isLoop);
		}
	}

	static void playEffectMusicRandom(Vector<String*> effectVec, bool isLoop)	//���Ч����Ч
	{
		bool music_on = GameMusicScene::effOn;
		if (music_on)
		{
			playEffectMusic(effectVec.at(rand() % effectVec.size())->getCString(), isLoop);
		}
	}

	static void stopAudioPlay()	//ֹͣ����
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
	}
};

#endif