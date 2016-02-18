#pragma once

#include <vector>
#include "Goblin.h"
#include "Enemy.h"
#include "Savage.h"
#include "SoundManager.h"

using namespace std;

extern Ogre::Timer waveTimer;//计时器
extern SoundManager* SoundMgr;

class Enemy;

class EnemyManager
{
public:
	EnemyManager(void);
	~EnemyManager(void);

	/// 用于创建敌人，每一关开始的时候调用一次，一次就把这关所有的敌人创建出来
	void createEnemy(int level,Ogre::SceneManager * m_pSceneMgr,double x, double y, double Range,int enemyNum);
	void createEnemy(Ogre::SceneManager * m_pSceneMgr,double x1, double x2, double y, double Range,int enemyNum1,int  enemyNum2);
	void createEnemy(Ogre::SceneManager * m_pSceneMgr,double x1, double x2, double y, double Range_x,double Range_y,int enemyNum1,int  enemyNum2);
	int findByName(string s);	/// 用于寻找某个特定的敌人，保留
	Enemy* DetectWhich();   /// ???
	void update(Vector3 TargetPosition,double timeSinceLastFrame);  /// 第一关，每帧在这个函数内将对所有敌人进行更新状态
	int update(Vector3 TargetPosition1,Vector3 TargetPosition2,Vector3 HeroPosition,double timeSinceLastFrame);  /// 第二关，每帧在这个函数内将对所有敌人进行更新状态

//private:
	std::vector<Enemy*> enemySet;
	int aliveNum;//目前场景中存活的敌人数量
	int defenseNum;//最多能抵御的敌人数量
};

