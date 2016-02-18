#include "EnemyManager.h"
#include "Timer.h"

#define RAND (rand()/32768.0)

EnemyManager::EnemyManager(void)
{
	//初始化能抵御的数量
	defenseNum=10;
	aliveNum=0;
}


EnemyManager::~EnemyManager(void)
{
}
/*  Author by GL */
/// 用于创建敌人，每一关开始的时候调用一次，一次就把这关所有的敌人创建出来
void EnemyManager::createEnemy(int level,Ogre::SceneManager * m_pSceneMgr,double x, double y, double Range,int enemyNum)
{
	enemySet.clear();
	aliveNum=enemyNum;
	int num=sqrt(float (enemyNum));
	double unit=2.0*Range/num;
	for (int i=0;i<num;i++)
		for (int j=0;j<enemyNum/num;j++)
		{
			double local_x=x-Range+unit*i+unit*0.1+RAND*unit*0.8;
			double local_y=y-Range+unit*j+unit*0.1+RAND*unit*0.8;
			enemySet.push_back(new Goblin(m_pSceneMgr,0.7,0.4,0.2,Vector3(local_x,-200,local_y)));
		}
	for (int i=0;i<enemyNum%num;i++)
	{
		double local_x=x-Range+unit*i+unit*0.1+RAND*unit*0.8;
		double local_y=y-Range+unit*num+unit*0.1+RAND*unit*0.8;
		enemySet.push_back(new Goblin(m_pSceneMgr,0.7,0.4,0.2,Vector3(local_x,-200,local_y)));
	}
}
void EnemyManager::createEnemy(Ogre::SceneManager * m_pSceneMgr,double x1, double x2, double y, double Range,int enemyNum1,int  enemyNum2)
{
	enemySet.clear();
	aliveNum=enemyNum1+enemyNum2;
	
	//左边门
	if(enemyNum1>0)
	{
		int num=sqrt(float (enemyNum1));
		double unit=2.0*Range/num;
		for (int i=0;i<num;i++)
			for (int j=0;j<enemyNum1/num;j++)
			{
				double local_x=x1-Range+unit*i+unit*0.1+RAND*unit*0.8;
				double local_y=y-Range+unit*j+unit*0.1+RAND*unit*0.8;
				enemySet.push_back(new Savage(m_pSceneMgr,6,0.4,0.2,Vector3(local_x,0,local_y)));
			}
		for (int i=0;i<enemyNum1%num;i++)
		{
			double local_x=x1-Range+unit*i+unit*0.1+RAND*unit*0.8;
			double local_y=y-Range+unit*num+unit*0.1+RAND*unit*0.8;
			enemySet.push_back(new Savage(m_pSceneMgr,6,0.4,0.2,Vector3(local_x,0,local_y)));
		}
	}
	//右边门
	if (enemyNum2>0)
	{
		int num=sqrt(float (enemyNum2));
		double unit=2.0*Range/num;
		for (int i=0;i<num;i++)
			for (int j=0;j<enemyNum2/num;j++)
			{
				double local_x=x2-Range+unit*i+unit*0.1+RAND*unit*0.8;
				double local_y=y-Range+unit*j+unit*0.1+RAND*unit*0.8;
				enemySet.push_back(new Savage(m_pSceneMgr,6,0.4,0.2,Vector3(local_x,0,local_y)));
			}
		for (int i=0;i<enemyNum2%num;i++)
		{
			double local_x=x2-Range+unit*i+unit*0.1+RAND*unit*0.8;
			double local_y=y-Range+unit*num+unit*0.1+RAND*unit*0.8;
			enemySet.push_back(new Savage(m_pSceneMgr,6,0.4,0.2,Vector3(local_x,0,local_y)));
		}
	}
}
void EnemyManager::createEnemy(Ogre::SceneManager * m_pSceneMgr,double x1, double x2, double y, double Range_x,double Range_y,int enemyNum1,int  enemyNum2)
{
	enemySet.clear();
	aliveNum=enemyNum1+enemyNum2;
	
	//左边门
	if(enemyNum1>0)
	{
		int num=3;
		double unit=2.0*Range_x/num;
		for (int i=0;i<num;i++)
			for (int j=0;j<enemyNum1/num;j++)
			{
				double local_x=x1-Range_x+unit*i+unit*0.1+RAND*unit*0.8;
				double local_y=y-unit*j+unit*0.1-RAND*unit*0.8;
				enemySet.push_back(new Savage(m_pSceneMgr,6,0.4,0.2,Vector3(local_x,0,local_y)));
			}
		for (int i=0;i<enemyNum1%num;i++)
		{
			double local_x=x1-Range_x+unit*i+unit*0.1+RAND*unit*0.8;
			double local_y=y-unit*num+unit*0.1-RAND*unit*0.8;
			enemySet.push_back(new Savage(m_pSceneMgr,6,0.4,0.2,Vector3(local_x,0,local_y)));
		}
	}
	//右边门
	if (enemyNum2>0)
	{
		int num=3;
		double unit=2.0*Range_x/num;
		for (int i=0;i<num;i++)
			for (int j=0;j<enemyNum2/num;j++)
			{
				double local_x=x2-Range_x+unit*i+unit*0.1+RAND*unit*0.8;
				double local_y=y-unit*j-unit*0.1-RAND*unit*0.8;
				enemySet.push_back(new Savage(m_pSceneMgr,6,0.4,0.2,Vector3(local_x,0,local_y)));
			}
		for (int i=0;i<enemyNum2%num;i++)
		{
			double local_x=x2-Range_x+unit*i+unit*0.1+RAND*unit*0.8;
			double local_y=y-unit*num-unit*0.1-RAND*unit*0.8;
			enemySet.push_back(new Savage(m_pSceneMgr,6,0.4,0.2,Vector3(local_x,0,local_y)));
		}
	}
}
/// 用于寻找某个特定的敌人
int EnemyManager::findByName(string s)
{
	char * sNum = new char(10);
	s.copy(sNum,s.length()-5,5);
	int TargetId=atoi(sNum);

	//二分查找,在队列寻找TargetId
	int left, right, middle;

    left = 0;
	right = enemySet.size() - 1;

    while (left <= right)
    {
        middle = (left + right) / 2;
		if (enemySet[middle]->EnemyId > TargetId)
        {
            right = middle - 1;
        }
        else if (enemySet[middle]->EnemyId < TargetId)
        {
            left = middle + 1;
        }
        else
        {
            return middle;
        }
    }

    return -1;
}

/// 每帧在这个函数内将对所有敌人进行更新状态，第一关，一个target
void EnemyManager::update(Vector3 TargetPosition,double timeSinceLastFrame)//	TargetPosition=Vector3(0,0,-250);
{
	float gravity = -20;
	for (int i=0;i<	enemySet.size();i++)
	{
		if (enemySet[i]->state==0)//如果活着
		{
			double Unit_t=timeSinceLastFrame/1000.0*enemySet[i]->speed;//Speed暂时为50
			//double Unit_t=50/1000.0*50;//Speed暂时为50
			double Distance=Vector3(enemySet[i]->scenenode->getPosition()).distance(TargetPosition);
			//已经冲进城门
			if (enemySet[i]->scenenode->getPosition().z<TargetPosition.z+5)
			{
				defenseNum--;
				//if (defenseNum<0)
				//{
				//	//MessageBox(0,"aa","aa",0);
				//	//gameover;
				//}
				//enemySet[i]->state=1;//DEAD;
				enemySet[i]->dead();
				aliveNum--;
				//最后一个敌人冲进城门，重新计时
				if(aliveNum==0)
				{
					waveTimer.reset();
					SoundMgr->play("count",1);
					SoundMgr->stop("fight");
				}

			}
			else//还在城门外
			{
				//判断是否快要到达城门					
				if ( Distance<=200)
				{
					//冲向城门
					//EnemyCube[i]->scenenode->lookAt(TargetPosition,Node::TS_PARENT, Vector3::UNIT_X);
					enemySet[i]->scenenode->lookAt(TargetPosition,Node::TransformSpace::TS_PARENT,Vector3::UNIT_X);
					double x=enemySet[i]->scenenode->getPosition().x-TargetPosition.x;
					double z=enemySet[i]->scenenode->getPosition().z-TargetPosition.z;
					//EnemyCube[i]->scenenode->translate(Vector3(-Unit_t*x/Distance,0,-Unit_t*z/Distance));
					enemySet[i]->controller->move(PxVec3(-Unit_t*x/Distance,gravity,-Unit_t*z/Distance), 0.0001, 1/60, PxControllerFilters());
					enemySet[i]->run(timeSinceLastFrame,2.5);
					
				}
				else
				{
					//继续向前走
					enemySet[i]->controller->move(PxVec3(0,gravity,-Unit_t), 0.0001, 1/60, PxControllerFilters());
					//EnemyCube[i]->scenenode->translate(Vector3(0,0,-Unit_t));
					enemySet[i]->run(timeSinceLastFrame,2.5);
				}
			}
		}
		else
			if (enemySet[i]->state==2)//如果被击中HIT
			{
				enemySet[i]->hit(timeSinceLastFrame);
			}
			else
				if (enemySet[i]->state==3)//如果确定死亡WILLDEAD
				{
					enemySet[i]->fallAnimation->setEnabled(true);
					enemySet[i]->willDead(timeSinceLastFrame);
				}

	}
	for (int i=0;i<enemySet.size();i++)
	{
		if (enemySet[i]->state==0)//如果活着
		{
			PxExtendedVec3 position = enemySet[i]->controller->getPosition();
			enemySet[i]->scenenode->setPosition(position.x,position.y-7,position.z);
		}
	}
}

//第二关，两个target更新敌人状态
int EnemyManager::update(Vector3 TargetPosition1,Vector3 TargetPosition2,Vector3 HeroPosition,double timeSinceLastFrame)//目标1进入走廊，目标2进入宫殿
{
	float gravity = -20;
	int attackNum=0;//英雄被攻击的次数
	double runScale=5;//跑步动画播放速度
	for (int i=0;i<	enemySet.size();i++)
	{
		if (enemySet[i]->state==Enemy::RUN)//如果活着
		{
			double Unit_t=timeSinceLastFrame/1000.0*enemySet[i]->speed;//Speed暂时为50

			//离英雄比较近，攻击
			if (Vector3(enemySet[i]->scenenode->getPosition()).distance(HeroPosition)<15 
				&& enemySet[i]->scenenode->getPosition().z<HeroPosition.z)
			{
				//ATTACK状态开始
				enemySet[i]->state=Enemy::ATTACK;//ATTACK
			}
			else//行走
			{
				//已经冲进走廊
				if (enemySet[i]->scenenode->getPosition().z>TargetPosition1.z-10-10*enemySet[i]->random)
				{
					if(enemySet[i]->scenenode->getPosition().z>TargetPosition2.z-5)//已经冲过走廊
					{
						defenseNum--;
						enemySet[i]->dead();
						aliveNum--;
						//最后一个敌人冲进城门，重新计时
						if(aliveNum==0)
						{
							waveTimer.reset();
							SoundMgr->play("count",1);
							SoundMgr->stop("fight");
						}
					}
					else//还在走廊内
					{		
						double Distance=Vector3(enemySet[i]->scenenode->getPosition()).distance(TargetPosition2);
						if ( Distance<=50)//靠近target2
						{
							//冲向target2
							enemySet[i]->scenenode->lookAt(TargetPosition2,Node::TransformSpace::TS_PARENT,Vector3::UNIT_Z);

							double x=enemySet[i]->scenenode->getPosition().x-TargetPosition2.x;
							double z=enemySet[i]->scenenode->getPosition().z-TargetPosition2.z;

							enemySet[i]->controller->move(PxVec3(-Unit_t*x/Distance,gravity,-Unit_t*z/Distance), 0.0001, 1/60, PxControllerFilters());
							enemySet[i]->run(timeSinceLastFrame,runScale);
						}							
						else//离target2较远，直走
							if (enemySet[i]->scenenode->getPosition().x<98)//如果卡在柱子内
							{
								//向右偏移
								enemySet[i]->scenenode->resetOrientation();
								enemySet[i]->scenenode->rotate(Ogre::Quaternion(Ogre::Degree( 90 ), Ogre::Vector3::UNIT_Y),Node::TransformSpace::TS_PARENT);
								enemySet[i]->controller->move(PxVec3(Unit_t,gravity,0), 0.0001, 1/60, PxControllerFilters());
								enemySet[i]->run(timeSinceLastFrame,runScale);
							}
							else						
								if (enemySet[i]->scenenode->getPosition().x>190)//如果卡在柱子内
								{
									//向左偏移
									enemySet[i]->scenenode->resetOrientation();
									enemySet[i]->scenenode->rotate(Ogre::Quaternion(Ogre::Degree( -90 ),Ogre::Vector3::UNIT_Y),Node::TransformSpace::TS_PARENT);
									enemySet[i]->controller->move(PxVec3(-Unit_t,gravity,0), 0.0001, 1/60, PxControllerFilters());
									enemySet[i]->run(timeSinceLastFrame,runScale);
								}
								else
								{
									//继续向前走
									enemySet[i]->scenenode->resetOrientation();
									enemySet[i]->controller->move(PxVec3(0,gravity,Unit_t), 0.0001, 1/60, PxControllerFilters());
									enemySet[i]->run(timeSinceLastFrame,runScale);
								}
					}
				}
				else//还在走廊外
				{
					Ogre::Vector3 localTarget=TargetPosition1+Ogre::Vector3(-40+80*enemySet[i]->random,0,0);

					double Distance=Vector3(enemySet[i]->scenenode->getPosition()).distance(localTarget);
					if(Distance<=150+40*enemySet[i]->random)//靠近taiget1
					{
						//冲向走廊
						enemySet[i]->scenenode->lookAt(TargetPosition1,Node::TransformSpace::TS_PARENT,Vector3::UNIT_Z);

						double x=enemySet[i]->scenenode->getPosition().x-localTarget.x;
						double z=enemySet[i]->scenenode->getPosition().z-localTarget.z;
					
						enemySet[i]->controller->move(PxVec3(-Unit_t*x/Distance,gravity,-Unit_t*z/Distance), 0.0001, 1/60, PxControllerFilters());
						enemySet[i]->run(timeSinceLastFrame,runScale);					
					}
					else
						if (enemySet[i]->scenenode->getPosition().x<-21)//如果卡在台阶内
						{
							//向右偏移
							enemySet[i]->scenenode->resetOrientation();
							enemySet[i]->scenenode->rotate(Ogre::Quaternion(Ogre::Degree( 90 ), Ogre::Vector3::UNIT_Y),Node::TransformSpace::TS_PARENT);
							enemySet[i]->controller->move(PxVec3(Unit_t,gravity,0), 0.0001, 1/60, PxControllerFilters());
							enemySet[i]->run(timeSinceLastFrame,runScale);
						}
						else						
							if (enemySet[i]->scenenode->getPosition().x>309)//如果卡在台阶内
							{
								//向左偏移
								enemySet[i]->scenenode->resetOrientation();
								enemySet[i]->scenenode->rotate(Ogre::Quaternion(Ogre::Degree( -90 ),Ogre::Vector3::UNIT_Y),Node::TransformSpace::TS_PARENT);
								enemySet[i]->controller->move(PxVec3(-Unit_t,gravity,0), 0.0001, 1/60, PxControllerFilters());
								enemySet[i]->run(timeSinceLastFrame,runScale);
							}
							else//离target1较远，直走
							{
								//继续向前走
								enemySet[i]->scenenode->resetOrientation();
								enemySet[i]->controller->move(PxVec3(0,gravity,Unit_t), 0.0001, 1/60, PxControllerFilters());
								enemySet[i]->run(timeSinceLastFrame,runScale);
							}
				}
			}
		}
		else
			if (enemySet[i]->state==Enemy::HIT)//如果被击中HIT
			{
				//HIT状态维持
				enemySet[i]->hit(timeSinceLastFrame);
			}
			else
				if (enemySet[i]->state==Enemy::WILLDEAD)//如果确定死亡WILLDEAD
				{
					//WILLDEAD状态维持
					enemySet[i]->willDead(timeSinceLastFrame);
				}
				else
					if(enemySet[i]->state==Enemy::ATTACK)//如果攻击英雄ATTACK
					{
						if (enemySet[i]->attack(timeSinceLastFrame)==true)//击中
						{
							attackNum++;
						}
						//离开英雄，继续行走
						if (Vector3(enemySet[i]->scenenode->getPosition()).distance(HeroPosition)>=15 
							|| enemySet[i]->scenenode->getPosition().z>=HeroPosition.z)//距离不够,或在后面
						{
							enemySet[i]->state=Enemy::RUN;//继续行走
							//停止当前攻击动画
							enemySet[i]->attackAnimation->setTimePosition(0.0);
							enemySet[i]->attackAnimation->setEnabled(false);
						}
					}
						else
						if (enemySet[i]->state==Enemy::WINDMAGIC)//被吹风魔法击中
						{
							double magnitude = 30*timeSinceLastFrame/1000.0;
							//冲向城门
							double x=enemySet[i]->scenenode->getPosition().x-HeroPosition.x;
							double z=enemySet[i]->scenenode->getPosition().z-HeroPosition.z;
							double dist = sqrt(x*x+z*z);
							enemySet[i]->controller->move(PxVec3(magnitude*x/dist,gravity,magnitude*z/dist), 0.0001, 1/60, PxControllerFilters());
							enemySet[i]->scenenode->lookAt(HeroPosition,Node::TransformSpace::TS_PARENT,Vector3::UNIT_Z);
							enemySet[i]->blowaway(timeSinceLastFrame);
							//enemySet[i]->hit(timeSinceLastFrame);
							//enemySet[i]->state=Enemy::RUN;
							//enemySet[i]->hitAnimation->setEnabled(true);
							//enemySet[i]->run(timeSinceLastFrame);
						}
	}
	for (int i=0;i<enemySet.size();i++)
	{
		if (enemySet[i]->state==Enemy::RUN||enemySet[i]->state==Enemy::WINDMAGIC)//如果活着
		{
			PxExtendedVec3 position = enemySet[i]->controller->getPosition();
			enemySet[i]->scenenode->setPosition(position.x,position.y,position.z);
		}
	}
	return attackNum;
}
/*  Author by GL */