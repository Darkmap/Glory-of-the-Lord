#pragma once


#include "OgreCamera.h"
#include "OgreEntity.h"
#include "OgreLogManager.h"
#include "OgreOverlay.h"
#include "OgreOverlayElement.h"
#include "OgreOverlayManager.h"
#include "OgreRoot.h"
#include "OgreViewport.h"
#include "OgreSceneManager.h"
#include "OgreRenderWindow.h"
#include "OgreConfigFile.h"

#include "OISEvents.h"
#include "OISInputManager.h"
#include "OISKeyboard.h"
#include "OISMouse.h"
	
#include "SdkTrays.h"
#include "PxPhysicsAPI.h"
#include "windows.h"
using namespace physx;
using namespace Ogre;

/*  Author by JH */

class Physx
{
public:
	Physx(void);
	~Physx(void);
	PxPhysics * mPhysics;
	PxScene * mScene;
	PxMaterial * mMaterial;
	PxCooking * mCooking;
	PxFoundation * mFoundation;
	PxControllerManager * manager;

	// ������̬����
	void createDynamicRigid(Entity * entity,int scale,Vector3 position,PxRigidDynamic *& actor,PxShape *& shape);
	// ������̬����
	void createStaticRigid(Entity * entity,float radians,Vector3 position,PxRigidStatic *& actor,PxShape *& shape);
	// ����Բ���ν�ɫ������
	void createCapsuleController(Entity * entity,float radius_scale,float height_scale,Vector3 position,float step_offset,PxController *& mController);
	// ������״��ɫ������
	void createBoxController(Entity * entity,float scale,Vector3 position,PxController *& mController);
	
	// �ͷŽ�ɫ�����ࣨ�����˱������ʱ�����,����������������Ƴ���
	void releaseController(PxController *& mController);


	// ��ʼ������������
	void start_simulate(float elapsedTime);
};

