#pragma once

#include "AdvancedOgreFramework.hpp"
#include "Ogre.h"

//class EnemyManager;



//陷阱抽象类
class Trap
{
public:
	
	Trap(void);
	~Trap(void);
	virtual void attack(){};
	virtual void invalid(){};

//protected:
	double space_x;//陷阱位置x坐标
	double space_y;//陷阱位置z坐标
	enum trap_Type//陷阱类型
	{
		SpikeFloor,//继承为地刺类
		SpikeWall,//继承为墙刺类
		FrozenTrap,//继承为冰霜陷阱类
		ARCHER//继承为弓箭手类
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
	int damage;//每次刺出对敌人的伤害
	double atkRange;//攻击范围，即地刺的宽
	Ogre::Timer cdTimer;
	double cdTime;
	double slowRate;//使冰冻范围内敌人移动速度降低的比率
	static int trapNum;
	int trapId;
};

//地刺，固定在地面，通过刺出伤害敌人，有冷却时间
class Spike_Floor :public Trap
{
public:
	Spike_Floor(Ogre::SceneManager *mSM ,Ogre::SceneNode *h,double x,double y,double scale);
	~Spike_Floor(void);
	static void attack();//进行攻击（播放动画）
	static void Init(Ogre::SceneManager *mSM,Ogre::SceneNode *h);//编辑动画，只运行一次

//private:
	int state;//陷阱状态，激活/冷却

	static Ogre::SceneNode * spikeNode;//放置刺的部分及动画
	static Ogre::AnimationState * mState;//刺出动画
};


//墙刺，固定在墙面上，通过射箭伤害敌人，有冷却时间
class Spike_Wall :public Trap
{
public:
	Spike_Wall(int x,int y);
	~Spike_Wall(void);
	void attack();

private:
	float cdTime;//冷却时间
	//int state;
	int damage;//每次发射对敌人的伤害
	int atkRange_x;//攻击范围，即墙刺的宽
	int atkRange_y;//离墙面特定距离
};

//冰霜陷阱，受到攻击后爆炸，使范围内敌人移动速度下降，并造成伤害
class Frozen_Trap :public Trap
{
public:
	Frozen_Trap(Ogre::SceneManager *mSceneMgr,Ogre::SceneNode *h,double x,double y,double scale);
	~Frozen_Trap(void);
	void attack();
	void invalid();
private:
	//float cdTime;//冷却时间
	//int state;//状态，未激活，爆炸后，消失
	//int damage;//爆炸后对周围敌人的伤害值
	//int atkRange;//爆炸后冰冻范围
	//int slowRate;//使冰冻范围内敌人移动速度降低的比率
};


//角色使用弓箭对特定方向射击
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

