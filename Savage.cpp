#include "Savage.h"
#define RAND (rand()/32768.0)
Savage::Savage(SceneManager * m_pSceneMgr,float ogre_scale,float physx_radius_scale,float physx_height_scale,Vector3 position)
{
	/*  Author by GL */
	random=RAND;
	speed=(4+random)*10;
	frozen=false;
	frozenRate=1.0;
	EnemyId=Enemy_num;
	char name[]="Enemy";
	char numCh[10];
	itoa(EnemyId,numCh,10);
	
	entity = m_pSceneMgr->createEntity(String(name)+numCh,"savage.mesh");

	
	entity->getMesh()->setAutoBuildEdgeLists(true);
	entity->setCastShadows(true);

	Enemy_num++;
	/*  Author by GL */

	scenenode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
	scenenode->scale(ogre_scale,ogre_scale,ogre_scale);
	scenenode->attachObject(entity);
	scenenode->setPosition(position);


	controller = NULL;
	float step_offset = 1.5;
	/// 创建 Physx 实体
	//mphysx->createDynamicRigid(entity,scale,position,actor,shape);
	mphysx->createCapsuleController(entity,physx_radius_scale,physx_height_scale,position,step_offset,controller);

	if(controller==NULL)
		MessageBox(0,"aa","aa",0);

	/*  Author by GL */
	//增加跑步动画
	runAnimation=entity->getAnimationState("savage_run");
	runAnimation->setEnabled(true);

	//增加击中动画
	hitAnimation=entity->getAnimationState("savage_shock");
	hitAnimation->setLoop(false);

	//增加倒地动画
	fallAnimation=entity->getAnimationState("savage_falldown");
	fallAnimation->setLoop(false);
	
	//增加攻击动画
	attackAnimation=entity->getAnimationState("savage_attack");
	attackAnimation->setLoop(false);

	blowAnimation=entity->getAnimationState("savage_blowaway");
	blowAnimation->setLoop(false);
	
	//初始化状态
	state=RUN;//RUN;
	hp=15;
	/*  Author by GL */

}


Savage::~Savage(void)
{
}
