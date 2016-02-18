#pragma once

#include "AdvancedOgreFramework.hpp"
#include "Ogre.h"

//class EnemyManager;



//���������
class Trap
{
public:
	
	Trap(void);
	~Trap(void);
	virtual void attack(){};
	virtual void invalid(){};

//protected:
	double space_x;//����λ��x����
	double space_y;//����λ��z����
	enum trap_Type//��������
	{
		SpikeFloor,//�̳�Ϊ�ش���
		SpikeWall,//�̳�Ϊǽ����
		FrozenTrap,//�̳�Ϊ��˪������
		ARCHER//�̳�Ϊ��������
	}trapType;
	enum State
	{
		Normal,
		Bomb,
		Invalid,
		WillInvalid,
		Busy
	}state;
	Ogre::SceneManager * manager;
	Ogre::Entity * entity;
	Ogre::SceneNode * headNode;
	Ogre::AnimationState * mState;
	int damage;//ÿ�δ̳��Ե��˵��˺�
	double atkRange;//������Χ�����ش̵Ŀ�
	Ogre::Timer cdTimer;
	double cdTime;
	double slowRate;//ʹ������Χ�ڵ����ƶ��ٶȽ��͵ı���
	static int trapNum;
	int trapId;
};

//�ش̣��̶��ڵ��棬ͨ���̳��˺����ˣ�����ȴʱ��
class Spike_Floor :public Trap
{
public:
	Spike_Floor(Ogre::SceneManager *mSM ,Ogre::SceneNode *h,double x,double y,double scale);
	~Spike_Floor(void);
	static void attack();//���й��������Ŷ�����
	static void Init(Ogre::SceneManager *mSM,Ogre::SceneNode *h);//�༭������ֻ����һ��

//private:
	int state;//����״̬������/��ȴ

	static Ogre::SceneNode * spikeNode;//���ô̵Ĳ��ּ�����
	static Ogre::AnimationState * mState;//�̳�����
};


//ǽ�̣��̶���ǽ���ϣ�ͨ������˺����ˣ�����ȴʱ��
class Spike_Wall :public Trap
{
public:
	Spike_Wall(int x,int y);
	~Spike_Wall(void);
	void attack();

private:
	float cdTime;//��ȴʱ��
	//int state;
	int damage;//ÿ�η���Ե��˵��˺�
	int atkRange_x;//������Χ����ǽ�̵Ŀ�
	int atkRange_y;//��ǽ���ض�����
};

//��˪���壬�ܵ�������ը��ʹ��Χ�ڵ����ƶ��ٶ��½���������˺�
class Frozen_Trap :public Trap
{
public:
	Frozen_Trap(Ogre::SceneManager *mSceneMgr,Ogre::SceneNode *h,double x,double y,double scale);
	~Frozen_Trap(void);
	void attack();
	void invalid();
private:
	//float cdTime;//��ȴʱ��
	//int state;//״̬��δ�����ը����ʧ
	//int damage;//��ը�����Χ���˵��˺�ֵ
	//int atkRange;//��ը�������Χ
	//int slowRate;//ʹ������Χ�ڵ����ƶ��ٶȽ��͵ı���
};


//��ɫʹ�ù������ض��������
class Archer :public Trap
{
public:
	Archer(Ogre::SceneManager *mSM ,Ogre::SceneNode *h,double x,double y,double scale);
	~Archer(void);
	void attack();
	void disappear();
private:
	int hp;
};

