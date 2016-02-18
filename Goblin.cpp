#include "Goblin.h"
#define RAND (rand()/32768.0)

Goblin::Goblin(SceneManager * m_pSceneMgr,float ogre_scale,float physx_radius_scale,float physx_height_scale,Vector3 position)
{
	/// ���� Ogreʵ��
	//entity = m_pSceneMgr->createEntity("gebulin.mesh");
	random=RAND;
	/*  Author by GL */
	speed=(7+random)*10;
	frozen=false;
	frozenRate=1.0;
	EnemyId=Enemy_num;
	char name[]="Enemy";
	char numCh[10];
	itoa(EnemyId,numCh,10);
	
	entity = m_pSceneMgr->createEntity(String(name)+numCh,"gubulinrun1 (2).mesh");

	entity->getMesh()->setAutoBuildEdgeLists(true);
	entity->setCastShadows(true);

	Enemy_num++;
	/*  Author by GL */

	scenenode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
	scenenode->scale(ogre_scale,ogre_scale,ogre_scale);
	scenenode->attachObject(entity);
	scenenode->setPosition(position);
	Quaternion q( Degree( 90 ), Vector3::UNIT_Y );
	scenenode->rotate(q);

	float step_offset = 2;
	/// ���� Physx ʵ��
	//mphysx->createDynamicRigid(entity,scale,position,actor,shape);
	mphysx->createCapsuleController(entity,physx_radius_scale,physx_height_scale,position,step_offset,controller);

	/*  Author by GL */
	//�����ܲ�����
	runAnimation=entity->getAnimationState("run");
	runAnimation->setEnabled(true);

	//���ӻ��ж���
	hitAnimation=entity->getAnimationState("hit");
	hitAnimation->setLoop(false);

	//���ӵ��ض���
	fallAnimation=entity->getAnimationState("down");
	fallAnimation->setLoop(false);

	//��ʼ��״̬
	state=RUN;//RUN;
	hp=10;
	/*  Author by GL */

}


Goblin::~Goblin(void)
{
}
