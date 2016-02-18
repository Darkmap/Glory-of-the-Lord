#pragma once

/*
	author by ���κ�
*/

#include "Trap.h"
#include <vector>
using namespace std;

class PropsManager
{
public:
	PropsManager(void);
	~PropsManager(void);
	void Init(Ogre::SceneManager * m_pSceneMgr);//��ʼ�����ڳ����������ɷ�������ĵ�ש
	void createSpikeFloor(Ogre::SceneManager * m_pSceneMgr,Ogre::Camera * m_pCamera,Ogre::RaySceneQuery * m_pRSQ);
	void createFrozenTrap(Ogre::SceneManager * m_pSceneMgr,Ogre::Camera * m_pCamera,Ogre::RaySceneQuery * m_pRSQ);
	void createArcher(Ogre::SceneManager * m_pSceneMgr,Ogre::Camera * m_pCamera,Ogre::RaySceneQuery * m_pRSQ);
//private:
	std::vector<Trap*> hurtTrapSet; //�洢�Ӵ��˺�������
	std::vector<Trap*> slowTrapSet;//�洢�Ӵ�����������
	std::vector<Trap*> bombTrapSet;//�洢�Ӵ���ը������
	std::vector<Trap*> archerSet;//�洢archer������
};
