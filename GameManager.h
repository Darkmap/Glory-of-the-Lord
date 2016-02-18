#pragma once

#include "EnemyManager.h"
#include "PropsManager.h"
#include "SoundManager.h"
class GameManager
{
public:
	GameManager(EnemyManager * EnemyMgr,PropsManager * PropsMgr);
	~GameManager(void);
	void update(double timeSinceLastFrame);  // ÿ֡���ô˺������ں����л���Ӧ���õ��ߣ����˹�������update����
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

