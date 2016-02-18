#include "Catapult.h"

#define RAND (rand()/32768.0)

Catapult::Catapult(Ogre::SceneManager *mSMgr,double x,double y,bool foll)
{
	noneTime=1000;
	waitTime=3000;
	willHurtTime=1500;
	hurtTime=1000;
	damage=45;
	atkRange=45;
	follow=foll;
	mSceneMgr=mSMgr;
	set_x=x;
	set_y=y;
	timer.reset();
	state=NONE;
	sceneNode=mSceneMgr->getRootSceneNode()->createChildSceneNode();
	hurtNode=mSceneMgr->getRootSceneNode()->createChildSceneNode();
	sceneNode->setPosition(set_x,-200,set_y);
	sceneNode->scale(20,20,20);
	entity=mSceneMgr->createEntity("catapult.mesh");
	entity->getMesh()->setAutoBuildEdgeLists(true);
	entity->setCastShadows(true);
	sceneNode->attachObject(entity);
	sceneNode->rotate(Ogre::Quaternion(Ogre::Degree( -90 ), Ogre::Vector3::UNIT_Y));
	AniState=entity->getAnimationState("catapult_shoot");
	//AniState->setLoop(false);
}


Catapult::~Catapult(void)
{
}
void Catapult::putIn(Ogre::Vector3 position,double Range_x,double Range_y)
{
	hurt_x=position.x-Range_x+2*Range_x*RAND;
	hurt_y=position.z-Range_y+2*Range_y*RAND;
	state=WAIT;
	timer.reset();
	///////////////////��ʼ���Ÿ�������Ч��///////////////////////
	hurtNode->setPosition(hurt_x,-210,hurt_y);
	hurtNode->attachObject(ParticleSystemTools::getRedRing(mSceneMgr));
}
bool Catapult::update(Ogre::Vector3 heroPosition ,double timeSinceLastFrame)
{
	if (state==NONE)
	{
		if (timer.getMilliseconds()>noneTime)
		{
			ifHurt=false;
			if (follow)
			{
				putIn(heroPosition,30,30);
			}
			else
			{
				putIn(Ogre::Vector3(0,0,-250),100,500);
			}
		}
	}
	if (state==WAIT)
	{
		AniState->setEnabled(true);
		AniState->addTime(timeSinceLastFrame/1000.0);
		if (timer.getMilliseconds()>waitTime)
		{
			state=WILLHURT;
			timer.reset();			
			////////////////////��ʼ����ʯ���һ�/////////////////////////////
			hurtNode->attachObject(ParticleSystemTools::getBang(mSceneMgr));
		}
	}
	if (state==WILLHURT)
	{
		AniState->setEnabled(false);
		AniState->setTimePosition(0.0);
		if (timer.getMilliseconds()>willHurtTime)
		{
			state=HURT;
			timer.reset();			
		}
	}
	if (state==HURT)
	{
		
		if (timer.getMilliseconds()>hurtTime)
		{
			state=NONE;
			timer.reset();
			///////////////ֹͣ����Ч����ֹͣ�һ�////////////////////////////
			hurtNode->detachObject(ParticleSystemTools::getBang(mSceneMgr));
			hurtNode->detachObject(ParticleSystemTools::getRedRing(mSceneMgr));
		}
		//�ж��˺�
		if (!ifHurt)//û�б��˺���������б��˺�������Ҫ�˺��ڶ��Σ�
		{
			if (heroPosition.distance(Ogre::Vector3(hurt_x,-200,hurt_y))<atkRange)//Բ������
			{
				//Ӣ�۱�����
				ifHurt=true;
				return true;
			}
		}
	}
	return false;
}