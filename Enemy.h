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
	void run(double timeSinceLastFrame,double scale);//可更改动画播放速度
	void hit(double timeSinceLastFrame);
	void willDead(double timeSinceLastFrame);//快要死亡，播放倒地动画
	bool attack(double timeSinceLastFrame);//攻击动画,攻击成功返回true
	void blowaway(double timeSinceLastFrame);
	void dead();//删除OGRE实体和物理模型
	void slow(double slowRate);
	void reset(double slowRate);

	AnimationState * runAnimation;
	AnimationState * hitAnimation;
	AnimationState * fallAnimation;
	AnimationState * attackAnimation;
	AnimationState * blowAnimation;
	int EnemyId;//每个敌人的唯一标识符
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

