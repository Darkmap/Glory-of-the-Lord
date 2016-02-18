#pragma once

#include "Ogre.h"

#include "ParticleSystem.hpp"

class Catapult
{
public:
	Catapult(Ogre::SceneManager *mSceneMgr,double x,double y,bool follow);
	~Catapult(void);
	void putIn(Ogre::Vector3 position,double Range_x,double Range_y);
	bool update(Ogre::Vector3 heroPosition ,double timeSinceLastFrame);
	double atkRange;
	double damage;
	bool follow;//是否跟踪英雄
	Ogre::Entity *entity;
	Ogre::SceneNode *sceneNode;
	Ogre::SceneNode *hurtNode;
	Ogre::ParticleSystem* ps;
	Ogre::SceneManager *mSceneMgr;
	Ogre::AnimationState *AniState;//投掷动画
	double set_x;//车的位置
	double set_y;
	double hurt_x;//伤害区域
	double hurt_y;
	bool ifHurt;
	enum STATE
	{
		NONE,
		WAIT,
		WILLHURT,
		HURT
	}state;
	Ogre::Timer timer;
	double noneTime;
	double waitTime;
	double hurtTime;
	double willHurtTime;
};

