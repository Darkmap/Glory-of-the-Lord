#include "PropsManager.h"

PropsManager::PropsManager(void)
{
}

PropsManager::~PropsManager(void)
{
}

//初始化，在场景中铺满可放置陷阱的地砖
void PropsManager::Init(Ogre::SceneManager * m_pSceneMgr)
{
	/*  Author by GL */
	//创建地砖ground
	
	Ogre::Plane ground(Ogre::Vector3::UNIT_Y, -10);//定义平面
	Ogre::MeshManager::getSingleton().createPlane("ground",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, ground,
		100, 100, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);//创建平面ground，大小100*100
		
	int groundNum=0;
	double pos=-6;//微调陷阱位置

	//走廊
	for (int i=0;i<3;i++)
	{
		for (int j=0;j<18;j++)
		{
		
			char name[]="ground";
			char num[10];
			itoa(groundNum,num,10);
			groundNum++;
			Ogre::Entity * entGround = m_pSceneMgr->createEntity( Ogre::String(name)+num,"ground");//实例化，实体ground
			Ogre::SceneNode * node=m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
			node->attachObject(entGround);//关联实体ground到场景
			node->setPosition(120+pos+30*i,-40.1,595+30*j);
			entGround->setMaterialName("Transparent");//添加纹理
			node->scale(0.3,0.3,0.3);
		}
	}
	//大厅
	for (int i=0;i<11;i++)
	{
		for (int j=0;j<4;j++)
		{
		
			char name[]="ground";
			char num[10];
			itoa(groundNum,num,10);
			groundNum++;
			Ogre::Entity * entGround = m_pSceneMgr->createEntity( Ogre::String(name)+num,"ground");//实例化，实体ground
			Ogre::SceneNode * node=m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
			node->attachObject(entGround);//关联实体ground到场景
			node->setPosition(0+pos+30*i,-40.1,475+30*j);
			entGround->setMaterialName("Transparent");//添加纹理
			node->scale(0.3,0.3,0.3);
		}
	}
	//大厅右
	for (int i=0;i<3;i++)
	{
		for (int j=0;j<6;j++)
		{
		
			char name[]="ground";
			char num[10];
			itoa(groundNum,num,10);
			groundNum++;
			Ogre::Entity * entGround = m_pSceneMgr->createEntity( Ogre::String(name)+num,"ground");//实例化，实体ground
			Ogre::SceneNode * node=m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
			node->attachObject(entGround);//关联实体ground到场景
			node->setPosition(240+pos+30*i,-40.1,295+30*j);
			entGround->setMaterialName("Transparent");//添加纹理
			node->scale(0.3,0.3,0.3);
		}
	}
	//大厅左
	for (int i=0;i<3;i++)
	{
		for (int j=0;j<6;j++)
		{
		
			char name[]="ground";
			char num[10];
			itoa(groundNum,num,10);
			groundNum++;
			Ogre::Entity * entGround = m_pSceneMgr->createEntity( Ogre::String(name)+num,"ground");//实例化，实体ground
			Ogre::SceneNode * node=m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
			node->attachObject(entGround);//关联实体ground到场景
			node->setPosition(0+pos+30*i,-40.1,295+30*j);
			entGround->setMaterialName("Transparent");//添加纹理
			node->scale(0.3,0.3,0.3);
		}
	}

	//初始化地刺陷阱
	Ogre::SceneNode * m_pOgreTrapNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("SpikeFLoorNode");
	Spike_Floor::Init(m_pSceneMgr,m_pOgreTrapNode);
	Spike_Floor::attack();
	/*  Author by GL */
}

void PropsManager::createSpikeFloor(Ogre::SceneManager * m_pSceneMgr,Ogre::Camera * m_pCamera,Ogre::RaySceneQuery * m_pRSQ)
{
	Ogre::Ray mouseRay = m_pCamera->getCameraToViewportRay(0.5,0.5);
	m_pRSQ->setRay(mouseRay);
	m_pRSQ->setSortByDistance(true);

	Ogre::RaySceneQueryResult &result = m_pRSQ->execute();
	Ogre::RaySceneQueryResult::iterator itr;

	for(itr = result.begin(); itr != result.end(); itr++)
	{
		if(itr->movable && itr->distance>0)
		{
			Ogre::SceneNode * m_pCurrentObject = m_pSceneMgr->getEntity(itr->movable->getName())->getParentSceneNode();
			//m_pCurrentObject->showBoundingBox(true);
			if(itr->movable->getName()[0]=='g')//是地面，ground
			{
				hurtTrapSet.push_back(new Spike_Floor(m_pSceneMgr,m_pCurrentObject,m_pCurrentObject->getPosition().x,m_pCurrentObject->getPosition().z,0.3));
			}	
			if(itr->movable->getName()[0]=='C')//是英雄，Cube
			{
				continue;//穿过
			}
			if(itr->movable->getName()[0]=='E')//是敌人，Enemy
			{
				continue;//穿过
			}
			break;
		}
	}
}
void PropsManager::createFrozenTrap(Ogre::SceneManager * m_pSceneMgr,Ogre::Camera * m_pCamera,Ogre::RaySceneQuery * m_pRSQ)
{
	Ogre::Ray mouseRay = m_pCamera->getCameraToViewportRay(0.5,0.5);
	m_pRSQ->setRay(mouseRay);
	m_pRSQ->setSortByDistance(true);

	Ogre::RaySceneQueryResult &result = m_pRSQ->execute();
	Ogre::RaySceneQueryResult::iterator itr;

	for(itr = result.begin(); itr != result.end(); itr++)
	{
		if(itr->movable && itr->distance>0)
		{
			Ogre::SceneNode * m_pCurrentObject = m_pSceneMgr->getEntity(itr->movable->getName())->getParentSceneNode();
			//m_pCurrentObject->showBoundingBox(true);
			if (itr->movable->getName()[0]=='g')//是地面，ground
			{			
				bombTrapSet.push_back(new Frozen_Trap(m_pSceneMgr,m_pCurrentObject,m_pCurrentObject->getPosition().x,m_pCurrentObject->getPosition().z,0.3));
			}	
			if (itr->movable->getName()[0]=='C')//是英雄，Cube
			{
				continue;//穿过
			}
			if (itr->movable->getName()[0]=='E')//是敌人，Enemy
			{
				continue;//穿过
			}
			break;
		}
	}

}
void PropsManager::createArcher(Ogre::SceneManager * m_pSceneMgr,Ogre::Camera * m_pCamera,Ogre::RaySceneQuery * m_pRSQ)
{
	Ogre::Ray mouseRay = m_pCamera->getCameraToViewportRay(0.5,0.5);
	m_pRSQ->setRay(mouseRay);
	m_pRSQ->setSortByDistance(true);

	Ogre::RaySceneQueryResult &result = m_pRSQ->execute();
	Ogre::RaySceneQueryResult::iterator itr;

	for(itr = result.begin(); itr != result.end(); itr++)
	{
		if(itr->movable && itr->distance>0)
		{
			Ogre::SceneNode * m_pCurrentObject = m_pSceneMgr->getEntity(itr->movable->getName())->getParentSceneNode();
			//m_pCurrentObject->showBoundingBox(true);
			if (itr->movable->getName()[0]=='g')//是地面，ground
			{			
				archerSet.push_back(new Archer(m_pSceneMgr,m_pCurrentObject,m_pCurrentObject->getPosition().x,m_pCurrentObject->getPosition().z,4));
			}	
			if (itr->movable->getName()[0]=='C')//是英雄，Cube
			{
				continue;//穿过
			}
			if (itr->movable->getName()[0]=='E')//是敌人，Enemy
			{
				continue;//穿过
			}
			break;
		}
	}
}