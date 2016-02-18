#include "Enemy.h"

/*  Author by GL */
#define RAND (rand()/32768.0)
int Enemy::Enemy_num=0;
/*  Author by GL */
Enemy::Enemy(void)
{
}


Enemy::~Enemy(void)
{
}
void Enemy::run(double timeSinceLastFrame)
{
	runAnimation->addTime(timeSinceLastFrame/1000.0*frozenRate);
}
void Enemy::run(double timeSinceLastFrame,double scale)
{
	runAnimation->addTime(timeSinceLastFrame/(950.0+100.0*random)*scale*frozenRate);
}
void Enemy::hit(double timeSinceLastFrame)
{
	hitAnimation->setEnabled(true);
	hitAnimation->addTime(timeSinceLastFrame/1000.0*frozenRate);
	if (hitAnimation->hasEnded())
	{
		state=Enemy::RUN;//RUN
		hitAnimation->setEnabled(false);
	}
}
void Enemy::willDead(double timeSinceLastFrame)
{
	fallAnimation->setEnabled(true);
	fallAnimation->addTime(timeSinceLastFrame/1000.0);
	if (fallAnimation->hasEnded())
	{
		fallAnimation->setEnabled(false);
		dead();
	}
}
bool Enemy::attack(double timeSinceLastFrame)
{
	attackAnimation->setEnabled(true);
	attackAnimation->addTime(timeSinceLastFrame/1000.0*frozenRate);
	if (attackAnimation->hasEnded())
	{
		attackAnimation->setTimePosition(0.0);
		return true;
	}
	return false;
}
void Enemy::blowaway(double timeSinceLastFrame)
{
	blowAnimation->setEnabled(true);
	blowAnimation->addTime(timeSinceLastFrame/1000.0);
	if (blowAnimation->hasEnded())
	{
		state=Enemy::RUN;//RUN
		blowAnimation->setEnabled(false);
		blowAnimation->setTimePosition(0.0);
	}
}
void Enemy::dead()
{
	scenenode->detachAllObjects();
	controller->release();
	state=Enemy::DEAD;//DEAD
}
void Enemy::slow(double slowRate)
{
	speed*=slowRate;
	frozenRate=slowRate;
	frozen=true;
}
void Enemy::reset(double slowRate)
{
	speed=speed/slowRate;
	frozen=false;
	frozenRate=1.0;
}