#pragma once

/*
	author by 王嘉豪
*/

#include "Trap.h"
#include <vector>
using namespace std;

class PropsManager
{
public:
	PropsManager(void);
	~PropsManager(void);
	void Init(Ogre::SceneManager * m_pSceneMgr);//初始化，在场景中铺满可放置陷阱的地砖
	void createSpikeFloor(Ogre::SceneManager * m_pSceneMgr,Ogre::Camera * m_pCamera,Ogre::RaySceneQuery * m_pRSQ);
	void createFrozenTrap(Ogre::SceneManager * m_pSceneMgr,Ogre::Camera * m_pCamera,Ogre::RaySceneQuery * m_pRSQ);
	void createArcher(Ogre::SceneManager * m_pSceneMgr,Ogre::Camera * m_pCamera,Ogre::RaySceneQuery * m_pRSQ);
//private:
	std::vector<Trap*> hurtTrapSet; //存储接触伤害型陷阱
	std::vector<Trap*> slowTrapSet;//存储接触减速型陷阱
	std::vector<Trap*> bombTrapSet;//存储接触爆炸型陷阱
	std::vector<Trap*> archerSet;//存储archer型陷阱
};
