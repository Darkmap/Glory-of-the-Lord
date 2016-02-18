#pragma once

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkTrays.h>
#include "PxPhysicsAPI.h"
#include "Physx.h"
using namespace physx;

extern Physx * mphysx;



class Enemy
{
public:
	Enemy(void);
	~Enemy(void);

	int hp;
	double speed;
	Entity * entity;
	SceneNode * scenenode;
	PxShape * shape;
	PxRigidDynamic * actor;
	PxController * controller;
	double random;

	/*  Author by GL */
	void run(double timeSinceLastFrame);
	void run(double timeSinceLastFrame,double scale);//�ɸ��Ķ��������ٶ�
	void hit(double timeSinceLastFrame);
	void willDead(double timeSinceLastFrame);//��Ҫ���������ŵ��ض���
	bool attack(double timeSinceLastFrame);//��������,�����ɹ�����true
	void blowaway(double timeSinceLastFrame);
	void dead();//ɾ��OGREʵ�������ģ��
	void slow(double slowRate);
	void reset(double slowRate);

	AnimationState * runAnimation;
	AnimationState * hitAnimation;
	AnimationState * fallAnimation;
	AnimationState * attackAnimation;
	AnimationState * blowAnimation;
	int EnemyId;//ÿ�����˵�Ψһ��ʶ��
	static int Enemy_num;
	/*  Author by GL */

	enum STATE
	{
		RUN,
		DEAD,
		HIT,
		WILLDEAD,
		ATTACK,
		WINDMAGIC
		//FROZEN
	}state;
	bool frozen;
	double frozenRate;
	//int state;//RUN,DEAD,HIT,WILLDEAD,ATTACK
};

