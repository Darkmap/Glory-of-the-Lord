#include "Trap.h"

Trap::Trap(void)
{
}
Trap::~Trap(void)
{
}
Ogre::SceneNode * Spike_Floor::spikeNode;//���ô̵Ĳ��ּ�����
Ogre::AnimationState * Spike_Floor::mState;//�̳�����
int Trap::trapNum=0;
Spike_Floor::Spike_Floor(Ogre::SceneManager *mSceneMgr,Ogre::SceneNode *h,double x,double y,double scale)
{
	//�����趨
	damage=8;
	atkRange=14.5;
	trapType=SpikeFloor;

	//���ݲ���
	space_x=x;
	space_y=y;
	headNode= h->createChildSceneNode();

	//���Ƶ���
	entity= mSceneMgr->createEntity("SpikeFloor_Floor.mesh");
	headNode->attachObject(entity);
	//headNode->setPosition(x,-10,y);//ѡ��ʽΪѡ�񳡾�����ʱ��ʹ�øþ�
	headNode->setPosition(0,-10.0,0);//ѡ��ʽΪѡ��ʵ���שʱ��ʹ�øþ�
	//���ƴ�
	Ogre::Entity* entSpike=  mSceneMgr->createEntity("SpikeFloor_Spike.mesh");
	Ogre::SceneNode * SpikeChild=spikeNode->createChildSceneNode();
	SpikeChild->setPosition(x,-50.0,y);
	SpikeChild->attachObject(entSpike);
	SpikeChild->scale(scale,scale,scale);
	
}
void Spike_Floor::Init(Ogre::SceneManager *mSM,Ogre::SceneNode *h)
{
	spikeNode = h->createChildSceneNode();
	//����̳�����
	Ogre::Animation* mAnim = mSM->createAnimation("SpikeAttack",2.0+2.0);//�̳�����ʱ��+��ȴʱ��
	mAnim->setInterpolationMode(Ogre::Animation::IM_LINEAR);
	Ogre::NodeAnimationTrack* t0=mAnim->createNodeTrack(1,spikeNode);//m_Node = "SpikeNode"
	
	Ogre::TransformKeyFrame* k_t0_0=t0->createNodeKeyFrame(0.0);//��ʼ 
	k_t0_0->setTranslate(Ogre::Vector3(0,0,0));//
	Ogre::TransformKeyFrame* k_t0_1=t0->createNodeKeyFrame(0.5); 
	k_t0_1->setTranslate(Ogre::Vector3(0,80*0.3,0));//����80���̳�
	Ogre::TransformKeyFrame* k_t0_2=t0->createNodeKeyFrame(1.5);
	k_t0_2->setTranslate(Ogre::Vector3(0,80*0.3,0));//ά��
	Ogre::TransformKeyFrame* k_t0_3=t0->createNodeKeyFrame(2.0); 
	k_t0_3->setTranslate(Ogre::Vector3(0,0,0));//����80����ԭ
 
	mState= mSM->createAnimationState("SpikeAttack");
}
Spike_Floor::~Spike_Floor(void)
{/*
	if (mSceneMgr)
		delete mSceneMgr;
	if(headNode)
		delete headNode;
	if(entity)
		delete entity;*/
}

void Spike_Floor::attack()
{
	mState->setEnabled(true);
}
//////////////////////////////////////////
Spike_Wall::Spike_Wall(int x,int y){}
Spike_Wall::~Spike_Wall(void){}
void Spike_Wall::attack(){}

Frozen_Trap::Frozen_Trap(Ogre::SceneManager *mSceneMgr,Ogre::SceneNode *h,double x,double y,double scale)
{
	//�����趨
	damage=20;
	atkRange=14;
	cdTime=10000.0;
	trapType=FrozenTrap;
	slowRate=0.3;
	state=Normal;
	trapId=trapNum;
	trapNum++;
	//���ݲ���
	space_x=x;
	space_y=y;

	manager=mSceneMgr;
	headNode= h->createChildSceneNode();

	//���Ƶ���
	entity= mSceneMgr->createEntity("IceFloor.mesh");
	headNode->attachObject(entity);
	//headNode->setPosition(x,-10,y);//ѡ��ʽΪѡ�񳡾�����ʱ��ʹ�øþ�
	headNode->setPosition(0,-10.0,0);//ѡ��ʽΪѡ��ʵ���שʱ��ʹ�øþ�
}
Frozen_Trap::~Frozen_Trap(void){}
void Frozen_Trap::attack()
{
	cdTimer.reset();
	state=Bomb;
	atkRange=45;
	headNode->detachObject(entity);
	//entity->~Entity();
	//entity=manager->createEntity("IceGround.mesh");
	//headNode->attachObject(entity);
	Ogre::ParticleSystem::setDefaultNonVisibleUpdateTimeout(5);  // set nonvisible timeout
	Ogre::ParticleSystem* ps;
	// create a green nimbus around the ogre head

	ps = manager->createParticleSystem("Particle"+Ogre::StringConverter::toString(trapId),"Examples/ringOfSnow");
	headNode->attachObject(ps);
}
void Frozen_Trap::invalid()
{
	headNode->detachAllObjects();
}

Archer::Archer(Ogre::SceneManager *mSM ,Ogre::SceneNode *h,double x,double y,double scale)
{
	damage=3;
	trapType=ARCHER;
	state=Normal;
	//����
	space_x=x;
	space_y=y;
	manager=mSM;
	headNode= h->createChildSceneNode();

	entity= mSM->createEntity("statue.mesh");
	headNode->attachObject(entity);
	//headNode->setPosition(x,-10,y);//ѡ��ʽΪѡ�񳡾�����ʱ��ʹ�øþ�
	headNode->setPosition(0,-10.0,0);//ѡ��ʽΪѡ��ʵ���שʱ��ʹ�øþ�
	mState=entity->getAnimationState("throw");
	mState->setLoop(false);
	headNode->scale(scale,scale,scale);
}
Archer::~Archer(void){}
void Archer::attack()
{
	state=Busy;
	mState->setEnabled(true);
}
