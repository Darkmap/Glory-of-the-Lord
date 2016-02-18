#pragma once

#include <vector>
#include "Goblin.h"
#include "Enemy.h"
#include "Savage.h"
#include "SoundManager.h"

using namespace std;

extern Ogre::Timer waveTimer;//��ʱ��
extern SoundManager* SoundMgr;

class Enemy;

class EnemyManager
{
public:
	EnemyManager(void);
	~EnemyManager(void);

	/// ���ڴ������ˣ�ÿһ�ؿ�ʼ��ʱ�����һ�Σ�һ�ξͰ�������еĵ��˴�������
	void createEnemy(int level,Ogre::SceneManager * m_pSceneMgr,double x, double y, double Range,int enemyNum);
	void createEnemy(Ogre::SceneManager * m_pSceneMgr,double x1, double x2, double y, double Range,int enemyNum1,int  enemyNum2);
	void createEnemy(Ogre::SceneManager * m_pSceneMgr,double x1, double x2, double y, double Range_x,double Range_y,int enemyNum1,int  enemyNum2);
	int findByName(string s);	/// ����Ѱ��ĳ���ض��ĵ��ˣ�����
	Enemy* DetectWhich();   /// ???
	void update(Vector3 TargetPosition,double timeSinceLastFrame);  /// ��һ�أ�ÿ֡����������ڽ������е��˽��и���״̬
	int update(Vector3 TargetPosition1,Vector3 TargetPosition2,Vector3 HeroPosition,double timeSinceLastFrame);  /// �ڶ��أ�ÿ֡����������ڽ������е��˽��и���״̬

//private:
	std::vector<Enemy*> enemySet;
	int aliveNum;//Ŀǰ�����д��ĵ�������
	int defenseNum;//����ܵ����ĵ�������
};

