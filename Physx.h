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

	// 创建动态刚体
	void createDynamicRigid(Entity * entity,int scale,Vector3 position,PxRigidDynamic *& actor,PxShape *& shape);
	// 创建静态刚体
	void createStaticRigid(Entity * entity,float radians,Vector3 position,PxRigidStatic *& actor,PxShape *& shape);
	// 创建圆柱形角色控制类
	void createCapsuleController(Entity * entity,float radius_scale,float height_scale,Vector3 position,float step_offset,PxController *& mController);
	// 创建盒状角色控制类
	void createBoxController(Entity * entity,float scale,Vector3 position,PxController *& mController);
	
	// 释放角色控制类（当敌人被消灭的时候调用,将其从物理世界中移除）
	void releaseController(PxController *& mController);


	// 开始计算物理世界
	void start_simulate(float elapsedTime);
};

