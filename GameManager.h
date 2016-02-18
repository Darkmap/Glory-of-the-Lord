#pragma once

#include "EnemyManager.h"
#include "PropsManager.h"
#include "SoundManager.h"
class GameManager
{
public:
	GameManager(EnemyManager * EnemyMgr,PropsManager * PropsMgr);
	~GameManager(void);
	void update(double timeSinceLastFrame);  // 每帧调用此函数，在函数中会相应调用道具，敌人管理器的update函数
	void updateHurtTrap();
	void updateSlowTrap();
	void updateBombTrap();
	void updateArcher(double timeSinceLastFrame);
//private:
	PropsManager * PropsMgr;
	EnemyManager * EnemyMgr;
	double cdTime;
	Ogre::Timer _cdTimer;
};

