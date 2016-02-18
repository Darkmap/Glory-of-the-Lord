#pragma once
#include "Enemy.h"

class Savage: public Enemy
{
public:
	Savage(SceneManager * m_pSceneMgr,float ogre_scale,float physx_radius_scale,float physx_height_scale,Vector3 position);
	~Savage(void);
};

