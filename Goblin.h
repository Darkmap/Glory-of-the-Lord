#pragma once
#include "Enemy.h"

class Goblin: public Enemy
{
public:
	Goblin(SceneManager * m_pSceneMgr,float ogre_scale,float physx_radius_scale,float physx_height_scale,Vector3 position);
	~Goblin(void);

	//void run(double timeSinceLastFrame);

};

