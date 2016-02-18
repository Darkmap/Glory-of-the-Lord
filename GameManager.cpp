#include "GameManager.h"

extern SoundManager * SoundMgr;

GameManager::GameManager(EnemyManager * enemymanager,PropsManager * propsmanager)
{
	EnemyMgr = enemymanager;
	PropsMgr = propsmanager;
	_cdTimer.reset();
	cdTime=2000;
}


GameManager::~GameManager(void)
{
}


// 每帧调用此函数，在函数中会相应调用道具，敌人管理器的update函数
void GameManager::update(double timeSinceLastFrame)
{
	updateHurtTrap();
	updateBombTrap();
	updateSlowTrap();
	updateArcher(timeSinceLastFrame);
}

void GameManager::updateHurtTrap()
{
	if (Spike_Floor::mState->getTimePosition()<1.5 && Spike_Floor::mState->getTimePosition()>0.5)
	//if (_cdTimer.getMilliseconds()>cdTime)
	{
		_cdTimer.reset();
		for (int i=0;i<EnemyMgr->enemySet.size();i++)
		{
			if (EnemyMgr->enemySet[i]->state!=Enemy::DEAD && EnemyMgr->enemySet[i]->state!=Enemy::WILLDEAD)//没有死，没有快死
			{
				for (int j=0;j<PropsMgr->hurtTrapSet.size();j++)
				{
					Ogre::Vector3 Enemy=EnemyMgr->enemySet[i]->scenenode->getPosition();
					Ogre::Vector3 Props=PropsMgr->hurtTrapSet[j]->headNode->getParent()->getPosition();
					double range = PropsMgr->hurtTrapSet[j]->atkRange;
					if (Props.x-range<=Enemy.x && Enemy.x<=Props.x+range
						&& Props.z-range<=Enemy.z && Enemy.z<=Props.z+range)
					{
						//HIT状态开始
						EnemyMgr->enemySet[i]->state=Enemy::HIT;
						EnemyMgr->enemySet[i]->hitAnimation->setTimePosition(0.0f);//重新开始播放

						EnemyMgr->enemySet[i]->hp-=PropsMgr->hurtTrapSet[j]->damage;
						
						if (EnemyMgr->enemySet[i]->hp<=0)
						{
							//WILLDEAD状态开始
							EnemyMgr->enemySet[i]->state=Enemy::WILLDEAD;//确定会死亡，WILLDEAD

							EnemyMgr->aliveNum--;
							//最后一个敌人被打死，重新计时
							if(EnemyMgr->aliveNum==0)
							{
								waveTimer.reset();
								SoundMgr->play("count",1);
								SoundMgr->stop("fight");
							}
						}
					}
				}
			}
		}
	}
}
void GameManager::updateBombTrap()
{
	for (int i=0;i<EnemyMgr->enemySet.size();i++)
		{
			if (EnemyMgr->enemySet[i]->state!=1 && EnemyMgr->enemySet[i]->state!=3)//没有死，没有快死
			{
				for (int j=0;j<PropsMgr->bombTrapSet.size();j++)
				{
					if (PropsMgr->bombTrapSet[j]->state==Trap::State::Normal)//陷阱有效
					{
						Ogre::Vector3 Enemy=EnemyMgr->enemySet[i]->scenenode->getPosition();
						Ogre::Vector3 Props=PropsMgr->bombTrapSet[j]->headNode->getParent()->getPosition();
						double range = PropsMgr->bombTrapSet[j]->atkRange;
						if (Props.x-range<=Enemy.x && Enemy.x<=Props.x+range
							&& Props.z-range<=Enemy.z && Enemy.z<=Props.z+range)
						{
							///处理敌人
							//HIT状态开始
							EnemyMgr->enemySet[i]->state=Enemy::HIT;//HIT;
							EnemyMgr->enemySet[i]->hitAnimation->setTimePosition(0.0f);

							EnemyMgr->enemySet[i]->hp-=PropsMgr->bombTrapSet[j]->damage;

							if (EnemyMgr->enemySet[i]->hp<=0)
							{
								EnemyMgr->enemySet[i]->state=Enemy::WILLDEAD;//确定会死亡，WILLDEAD

								EnemyMgr->aliveNum--;
								//最后一个敌人被打死，重新计时
								if(EnemyMgr->aliveNum==0)
								{
									waveTimer.reset();
									SoundMgr->play("count",1);
									SoundMgr->stop("fight");
								}
							}
							//处理陷阱
							PropsMgr->slowTrapSet.push_back(PropsMgr->bombTrapSet[j]);
							PropsMgr->bombTrapSet[j]->attack();
						}
					}
				}
			}
		}
}

void GameManager::updateSlowTrap()
{
	for (int j=0;j<PropsMgr->slowTrapSet.size();j++)
	{
		if (PropsMgr->slowTrapSet[j]->state==Trap::Bomb)
		{
			for (int i=0;i<EnemyMgr->enemySet.size();i++)
			{
				if (EnemyMgr->enemySet[i]->state!=1 && EnemyMgr->enemySet[i]->state!=3)//没有死，没有快死
				{
			
					Ogre::Vector3 Enemy=EnemyMgr->enemySet[i]->scenenode->getPosition();
					Ogre::Vector3 Props=PropsMgr->slowTrapSet[j]->headNode->getParent()->getPosition();
					double range = PropsMgr->slowTrapSet[j]->atkRange;
					if (Props.x-range<=Enemy.x && Enemy.x<=Props.x+range
						&& Props.z-range<=Enemy.z && Enemy.z<=Props.z+range)
					{
						if (!EnemyMgr->enemySet[i]->frozen)
						{
							EnemyMgr->enemySet[i]->slow(PropsMgr->slowTrapSet[j]->slowRate);
							EnemyMgr->enemySet[i]->frozen=true;
						}
					}
					else
					{
						if (EnemyMgr->enemySet[i]->frozen)
						{
							EnemyMgr->enemySet[i]->reset(PropsMgr->slowTrapSet[j]->slowRate);
							EnemyMgr->enemySet[i]->frozen=false;
						}
					}
					if (PropsMgr->slowTrapSet[j]->cdTimer.getMilliseconds()>PropsMgr->slowTrapSet[j]->cdTime)
					{
						PropsMgr->slowTrapSet[j]->state=Trap::WillInvalid;
						PropsMgr->slowTrapSet[j]->invalid();
					}
				}
				
			}
		}
		if (PropsMgr->slowTrapSet[j]->state==Trap::WillInvalid)
		{
			PropsMgr->slowTrapSet[j]->state=Trap::Invalid;
			for (int i=0;i<EnemyMgr->enemySet.size();i++)
			{
				if (EnemyMgr->enemySet[i]->state!=1 && EnemyMgr->enemySet[i]->state!=3)//没有死，没有快死
				{
			
					Ogre::Vector3 Enemy=EnemyMgr->enemySet[i]->scenenode->getPosition();
					Ogre::Vector3 Props=PropsMgr->slowTrapSet[j]->headNode->getParent()->getPosition();
					double range = PropsMgr->slowTrapSet[j]->atkRange;
					if (Props.x-range<=Enemy.x && Enemy.x<=Props.x+range
						&& Props.z-range<=Enemy.z && Enemy.z<=Props.z+range)
					{
						if (EnemyMgr->enemySet[i]->frozen)
						{
							EnemyMgr->enemySet[i]->reset(PropsMgr->slowTrapSet[j]->slowRate);
							EnemyMgr->enemySet[i]->frozen=false;
						}
					}
				}
			}
		}
	}
}
void GameManager::updateArcher(double timeSinceLastFrame)
{
	for (int j=0;j<PropsMgr->archerSet.size();j++)
	{

		if(PropsMgr->archerSet[j]->state==Trap::Normal)
		{
			double min=100;
		    int attacked=-1;
			for (int i=0;i<EnemyMgr->enemySet.size();i++)
			{
				if (EnemyMgr->enemySet[i]->state!=1 && EnemyMgr->enemySet[i]->state!=3)//没有死，没有快死
				{
					Ogre::Vector3 Enemy=EnemyMgr->enemySet[i]->scenenode->getPosition();
		            Ogre::Vector3 Props=PropsMgr->archerSet[j]->headNode->getParent()->getPosition();
					double source=sqrt(double (pow((Enemy.x)-(Props.x),2)+pow((Enemy.z)-(Props.z),2)));
					if(source<min)
					{
						min=source;
					    attacked=i;
					}
				}
			}
			//如果有敌人靠近
			if (attacked!=-1)
			{
				PropsMgr->archerSet[j]->attack();
				PropsMgr->archerSet[j]->headNode->resetOrientation();
				//PropsMgr->archerSet[j]->headNode->lookAt(EnemyMgr->enemySet[attacked]->scenenode->getPosition()+Vector3(0,0,0),
				//										Node::TransformSpace::TS_WORLD,Vector3::UNIT_Z);
				double x = EnemyMgr->enemySet[attacked]->scenenode->getPosition().x - PropsMgr->archerSet[j]->headNode->getParent()->getPosition().x;
				double z = EnemyMgr->enemySet[attacked]->scenenode->getPosition().z - PropsMgr->archerSet[j]->headNode->getParent()->getPosition().z;
				if (z>0)
				PropsMgr->archerSet[j]->headNode->rotate(Ogre::Quaternion(Ogre::Radian(atan(x/z)), Ogre::Vector3::UNIT_Y));
				else
					PropsMgr->archerSet[j]->headNode->rotate(Ogre::Quaternion(Ogre::Radian(Ogre::Math::PI+atan(x/z)), Ogre::Vector3::UNIT_Y));


				EnemyMgr->enemySet[attacked]->state=Enemy::HIT;//HIT;
				EnemyMgr->enemySet[attacked]->hitAnimation->setTimePosition(0.0f);

				EnemyMgr->enemySet[attacked]->hp-=PropsMgr->archerSet[j]->damage;
			
				if (EnemyMgr->enemySet[attacked]->hp<=0)
				{
					EnemyMgr->enemySet[attacked]->state=Enemy::WILLDEAD;//确定会死亡，WILLDEAD
					EnemyMgr->aliveNum--;
					//最后一个敌人被打死，重新计时
					if(EnemyMgr->aliveNum==0)
					{
						waveTimer.reset();
						SoundMgr->play("count",1);
						SoundMgr->stop("fight");
					}
				}
			}
		}
		if(PropsMgr->archerSet[j]->state==Trap::Busy)
		{
			PropsMgr->archerSet[j]->mState->addTime(timeSinceLastFrame/1000.0);
			if (PropsMgr->archerSet[j]->mState->hasEnded())
			{
				PropsMgr->archerSet[j]->mState->setTimePosition(0.0);
				PropsMgr->archerSet[j]->mState->setEnabled(false);
				PropsMgr->archerSet[j]->state=Trap::Normal;
			}
		}
	}
}

