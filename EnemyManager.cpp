#include "EnemyManager.h"
#include "Timer.h"

#define RAND (rand()/32768.0)

EnemyManager::EnemyManager(void)
{
	//��ʼ���ܵ���������
	defenseNum=10;
	aliveNum=0;
}


EnemyManager::~EnemyManager(void)
{
}
/*  Author by GL */
/// ���ڴ������ˣ�ÿһ�ؿ�ʼ��ʱ�����һ�Σ�һ�ξͰ�������еĵ��˴�������
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
	
	//�����
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
	//�ұ���
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
	
	//�����
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
	//�ұ���
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
/// ����Ѱ��ĳ���ض��ĵ���
int EnemyManager::findByName(string s)
{
	char * sNum = new char(10);
	s.copy(sNum,s.length()-5,5);
	int TargetId=atoi(sNum);

	//���ֲ���,�ڶ���Ѱ��TargetId
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

/// ÿ֡����������ڽ������е��˽��и���״̬����һ�أ�һ��target
void EnemyManager::update(Vector3 TargetPosition,double timeSinceLastFrame)//	TargetPosition=Vector3(0,0,-250);
{
	float gravity = -20;
	for (int i=0;i<	enemySet.size();i++)
	{
		if (enemySet[i]->state==0)//�������
		{
			double Unit_t=timeSinceLastFrame/1000.0*enemySet[i]->speed;//Speed��ʱΪ50
			//double Unit_t=50/1000.0*50;//Speed��ʱΪ50
			double Distance=Vector3(enemySet[i]->scenenode->getPosition()).distance(TargetPosition);
			//�Ѿ��������
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
				//���һ�����˳�����ţ����¼�ʱ
				if(aliveNum==0)
				{
					waveTimer.reset();
					SoundMgr->play("count",1);
					SoundMgr->stop("fight");
				}

			}
			else//���ڳ�����
			{
				//�ж��Ƿ��Ҫ�������					
				if ( Distance<=200)
				{
					//�������
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
					//������ǰ��
					enemySet[i]->controller->move(PxVec3(0,gravity,-Unit_t), 0.0001, 1/60, PxControllerFilters());
					//EnemyCube[i]->scenenode->translate(Vector3(0,0,-Unit_t));
					enemySet[i]->run(timeSinceLastFrame,2.5);
				}
			}
		}
		else
			if (enemySet[i]->state==2)//���������HIT
			{
				enemySet[i]->hit(timeSinceLastFrame);
			}
			else
				if (enemySet[i]->state==3)//���ȷ������WILLDEAD
				{
					enemySet[i]->fallAnimation->setEnabled(true);
					enemySet[i]->willDead(timeSinceLastFrame);
				}

	}
	for (int i=0;i<enemySet.size();i++)
	{
		if (enemySet[i]->state==0)//�������
		{
			PxExtendedVec3 position = enemySet[i]->controller->getPosition();
			enemySet[i]->scenenode->setPosition(position.x,position.y-7,position.z);
		}
	}
}

//�ڶ��أ�����target���µ���״̬
int EnemyManager::update(Vector3 TargetPosition1,Vector3 TargetPosition2,Vector3 HeroPosition,double timeSinceLastFrame)//Ŀ��1�������ȣ�Ŀ��2���빬��
{
	float gravity = -20;
	int attackNum=0;//Ӣ�۱������Ĵ���
	double runScale=5;//�ܲ����������ٶ�
	for (int i=0;i<	enemySet.size();i++)
	{
		if (enemySet[i]->state==Enemy::RUN)//�������
		{
			double Unit_t=timeSinceLastFrame/1000.0*enemySet[i]->speed;//Speed��ʱΪ50

			//��Ӣ�۱ȽϽ�������
			if (Vector3(enemySet[i]->scenenode->getPosition()).distance(HeroPosition)<15 
				&& enemySet[i]->scenenode->getPosition().z<HeroPosition.z)
			{
				//ATTACK״̬��ʼ
				enemySet[i]->state=Enemy::ATTACK;//ATTACK
			}
			else//����
			{
				//�Ѿ��������
				if (enemySet[i]->scenenode->getPosition().z>TargetPosition1.z-10-10*enemySet[i]->random)
				{
					if(enemySet[i]->scenenode->getPosition().z>TargetPosition2.z-5)//�Ѿ��������
					{
						defenseNum--;
						enemySet[i]->dead();
						aliveNum--;
						//���һ�����˳�����ţ����¼�ʱ
						if(aliveNum==0)
						{
							waveTimer.reset();
							SoundMgr->play("count",1);
							SoundMgr->stop("fight");
						}
					}
					else//����������
					{		
						double Distance=Vector3(enemySet[i]->scenenode->getPosition()).distance(TargetPosition2);
						if ( Distance<=50)//����target2
						{
							//����target2
							enemySet[i]->scenenode->lookAt(TargetPosition2,Node::TransformSpace::TS_PARENT,Vector3::UNIT_Z);

							double x=enemySet[i]->scenenode->getPosition().x-TargetPosition2.x;
							double z=enemySet[i]->scenenode->getPosition().z-TargetPosition2.z;

							enemySet[i]->controller->move(PxVec3(-Unit_t*x/Distance,gravity,-Unit_t*z/Distance), 0.0001, 1/60, PxControllerFilters());
							enemySet[i]->run(timeSinceLastFrame,runScale);
						}							
						else//��target2��Զ��ֱ��
							if (enemySet[i]->scenenode->getPosition().x<98)//�������������
							{
								//����ƫ��
								enemySet[i]->scenenode->resetOrientation();
								enemySet[i]->scenenode->rotate(Ogre::Quaternion(Ogre::Degree( 90 ), Ogre::Vector3::UNIT_Y),Node::TransformSpace::TS_PARENT);
								enemySet[i]->controller->move(PxVec3(Unit_t,gravity,0), 0.0001, 1/60, PxControllerFilters());
								enemySet[i]->run(timeSinceLastFrame,runScale);
							}
							else						
								if (enemySet[i]->scenenode->getPosition().x>190)//�������������
								{
									//����ƫ��
									enemySet[i]->scenenode->resetOrientation();
									enemySet[i]->scenenode->rotate(Ogre::Quaternion(Ogre::Degree( -90 ),Ogre::Vector3::UNIT_Y),Node::TransformSpace::TS_PARENT);
									enemySet[i]->controller->move(PxVec3(-Unit_t,gravity,0), 0.0001, 1/60, PxControllerFilters());
									enemySet[i]->run(timeSinceLastFrame,runScale);
								}
								else
								{
									//������ǰ��
									enemySet[i]->scenenode->resetOrientation();
									enemySet[i]->controller->move(PxVec3(0,gravity,Unit_t), 0.0001, 1/60, PxControllerFilters());
									enemySet[i]->run(timeSinceLastFrame,runScale);
								}
					}
				}
				else//����������
				{
					Ogre::Vector3 localTarget=TargetPosition1+Ogre::Vector3(-40+80*enemySet[i]->random,0,0);

					double Distance=Vector3(enemySet[i]->scenenode->getPosition()).distance(localTarget);
					if(Distance<=150+40*enemySet[i]->random)//����taiget1
					{
						//��������
						enemySet[i]->scenenode->lookAt(TargetPosition1,Node::TransformSpace::TS_PARENT,Vector3::UNIT_Z);

						double x=enemySet[i]->scenenode->getPosition().x-localTarget.x;
						double z=enemySet[i]->scenenode->getPosition().z-localTarget.z;
					
						enemySet[i]->controller->move(PxVec3(-Unit_t*x/Distance,gravity,-Unit_t*z/Distance), 0.0001, 1/60, PxControllerFilters());
						enemySet[i]->run(timeSinceLastFrame,runScale);					
					}
					else
						if (enemySet[i]->scenenode->getPosition().x<-21)//�������̨����
						{
							//����ƫ��
							enemySet[i]->scenenode->resetOrientation();
							enemySet[i]->scenenode->rotate(Ogre::Quaternion(Ogre::Degree( 90 ), Ogre::Vector3::UNIT_Y),Node::TransformSpace::TS_PARENT);
							enemySet[i]->controller->move(PxVec3(Unit_t,gravity,0), 0.0001, 1/60, PxControllerFilters());
							enemySet[i]->run(timeSinceLastFrame,runScale);
						}
						else						
							if (enemySet[i]->scenenode->getPosition().x>309)//�������̨����
							{
								//����ƫ��
								enemySet[i]->scenenode->resetOrientation();
								enemySet[i]->scenenode->rotate(Ogre::Quaternion(Ogre::Degree( -90 ),Ogre::Vector3::UNIT_Y),Node::TransformSpace::TS_PARENT);
								enemySet[i]->controller->move(PxVec3(-Unit_t,gravity,0), 0.0001, 1/60, PxControllerFilters());
								enemySet[i]->run(timeSinceLastFrame,runScale);
							}
							else//��target1��Զ��ֱ��
							{
								//������ǰ��
								enemySet[i]->scenenode->resetOrientation();
								enemySet[i]->controller->move(PxVec3(0,gravity,Unit_t), 0.0001, 1/60, PxControllerFilters());
								enemySet[i]->run(timeSinceLastFrame,runScale);
							}
				}
			}
		}
		else
			if (enemySet[i]->state==Enemy::HIT)//���������HIT
			{
				//HIT״̬ά��
				enemySet[i]->hit(timeSinceLastFrame);
			}
			else
				if (enemySet[i]->state==Enemy::WILLDEAD)//���ȷ������WILLDEAD
				{
					//WILLDEAD״̬ά��
					enemySet[i]->willDead(timeSinceLastFrame);
				}
				else
					if(enemySet[i]->state==Enemy::ATTACK)//�������Ӣ��ATTACK
					{
						if (enemySet[i]->attack(timeSinceLastFrame)==true)//����
						{
							attackNum++;
						}
						//�뿪Ӣ�ۣ���������
						if (Vector3(enemySet[i]->scenenode->getPosition()).distance(HeroPosition)>=15 
							|| enemySet[i]->scenenode->getPosition().z>=HeroPosition.z)//���벻��,���ں���
						{
							enemySet[i]->state=Enemy::RUN;//��������
							//ֹͣ��ǰ��������
							enemySet[i]->attackAnimation->setTimePosition(0.0);
							enemySet[i]->attackAnimation->setEnabled(false);
						}
					}
						else
						if (enemySet[i]->state==Enemy::WINDMAGIC)//������ħ������
						{
							double magnitude = 30*timeSinceLastFrame/1000.0;
							//�������
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
		if (enemySet[i]->state==Enemy::RUN||enemySet[i]->state==Enemy::WINDMAGIC)//�������
		{
			PxExtendedVec3 position = enemySet[i]->controller->getPosition();
			enemySet[i]->scenenode->setPosition(position.x,position.y,position.z);
		}
	}
	return attackNum;
}
/*  Author by GL */