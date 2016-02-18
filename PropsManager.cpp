#include "PropsManager.h"

PropsManager::PropsManager(void)
{
}

PropsManager::~PropsManager(void)
{
}

//��ʼ�����ڳ����������ɷ�������ĵ�ש
void PropsManager::Init(Ogre::SceneManager * m_pSceneMgr)
{
	/*  Author by GL */
	//������שground
	
	Ogre::Plane ground(Ogre::Vector3::UNIT_Y, -10);//����ƽ��
	Ogre::MeshManager::getSingleton().createPlane("ground",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, ground,
		100, 100, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);//����ƽ��ground����С100*100
		
	int groundNum=0;
	double pos=-6;//΢������λ��

	//����
	for (int i=0;i<3;i++)
	{
		for (int j=0;j<18;j++)
		{
		
			char name[]="ground";
			char num[10];
			itoa(groundNum,num,10);
			groundNum++;
			Ogre::Entity * entGround = m_pSceneMgr->createEntity( Ogre::String(name)+num,"ground");//ʵ������ʵ��ground
			Ogre::SceneNode * node=m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
			node->attachObject(entGround);//����ʵ��ground������
			node->setPosition(120+pos+30*i,-40.1,595+30*j);
			entGround->setMaterialName("Transparent");//�������
			node->scale(0.3,0.3,0.3);
		}
	}
	//����
	for (int i=0;i<11;i++)
	{
		for (int j=0;j<4;j++)
		{
		
			char name[]="ground";
			char num[10];
			itoa(groundNum,num,10);
			groundNum++;
			Ogre::Entity * entGround = m_pSceneMgr->createEntity( Ogre::String(name)+num,"ground");//ʵ������ʵ��ground
			Ogre::SceneNode * node=m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
			node->attachObject(entGround);//����ʵ��ground������
			node->setPosition(0+pos+30*i,-40.1,475+30*j);
			entGround->setMaterialName("Transparent");//�������
			node->scale(0.3,0.3,0.3);
		}
	}
	//������
	for (int i=0;i<3;i++)
	{
		for (int j=0;j<6;j++)
		{
		
			char name[]="ground";
			char num[10];
			itoa(groundNum,num,10);
			groundNum++;
			Ogre::Entity * entGround = m_pSceneMgr->createEntity( Ogre::String(name)+num,"ground");//ʵ������ʵ��ground
			Ogre::SceneNode * node=m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
			node->attachObject(entGround);//����ʵ��ground������
			node->setPosition(240+pos+30*i,-40.1,295+30*j);
			entGround->setMaterialName("Transparent");//�������
			node->scale(0.3,0.3,0.3);
		}
	}
	//������
	for (int i=0;i<3;i++)
	{
		for (int j=0;j<6;j++)
		{
		
			char name[]="ground";
			char num[10];
			itoa(groundNum,num,10);
			groundNum++;
			Ogre::Entity * entGround = m_pSceneMgr->createEntity( Ogre::String(name)+num,"ground");//ʵ������ʵ��ground
			Ogre::SceneNode * node=m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
			node->attachObject(entGround);//����ʵ��ground������
			node->setPosition(0+pos+30*i,-40.1,295+30*j);
			entGround->setMaterialName("Transparent");//�������
			node->scale(0.3,0.3,0.3);
		}
	}

	//��ʼ���ش�����
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
			if(itr->movable->getName()[0]=='g')//�ǵ��棬ground
			{
				hurtTrapSet.push_back(new Spike_Floor(m_pSceneMgr,m_pCurrentObject,m_pCurrentObject->getPosition().x,m_pCurrentObject->getPosition().z,0.3));
			}	
			if(itr->movable->getName()[0]=='C')//��Ӣ�ۣ�Cube
			{
				continue;//����
			}
			if(itr->movable->getName()[0]=='E')//�ǵ��ˣ�Enemy
			{
				continue;//����
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
			if (itr->movable->getName()[0]=='g')//�ǵ��棬ground
			{			
				bombTrapSet.push_back(new Frozen_Trap(m_pSceneMgr,m_pCurrentObject,m_pCurrentObject->getPosition().x,m_pCurrentObject->getPosition().z,0.3));
			}	
			if (itr->movable->getName()[0]=='C')//��Ӣ�ۣ�Cube
			{
				continue;//����
			}
			if (itr->movable->getName()[0]=='E')//�ǵ��ˣ�Enemy
			{
				continue;//����
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
			if (itr->movable->getName()[0]=='g')//�ǵ��棬ground
			{			
				archerSet.push_back(new Archer(m_pSceneMgr,m_pCurrentObject,m_pCurrentObject->getPosition().x,m_pCurrentObject->getPosition().z,4));
			}	
			if (itr->movable->getName()[0]=='C')//��Ӣ�ۣ�Cube
			{
				continue;//����
			}
			if (itr->movable->getName()[0]=='E')//�ǵ��ˣ�Enemy
			{
				continue;//����
			}
			break;
		}
	}
}