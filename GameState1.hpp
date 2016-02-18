//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef GAME_STATE_1_HPP
#define GAME_STATE_1_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AppState.hpp"
#include "PxPhysicsAPI.h"
#include "DotSceneLoader.hpp"
#include "Physx.h"
#include <OgreSubEntity.h>
#include <OgreMaterialManager.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <CEGUI/ImageManager.h>

/*  Author by JH */
#include "EnemyManager.h"
#include "SoundManager.h"
#include "Goblin.h"
#include "Enemy.h"
/*  Author by JH */
#include "Catapult.h"
#include "ParticleSystem.hpp"

using namespace physx;

extern Physx * mphysx;



//|||||||||||||||||||||||||||||||||||||||||||||||
/*
enum QueryFlags
{
	OGRE_HEAD_MASK	= 1<<0,
	CUBE_MASK		= 1<<1
};
*/

//|||||||||||||||||||||||||||||||||||||||||||||||

class GameState1 : public AppState
{
public:
	GameState1();

	DECLARE_APPSTATE_CLASS(GameState1)

	void enter();
	void createScene();
	void exit();
	bool pause();
	void resume();

	void moveCamera(double timeSinceLastFrame);
	void getInput(double timeSinceLastFrame);
	void buildGUI();

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

	void onLeftPressed(const OIS::MouseEvent &evt);
	void itemSelected(OgreBites::SelectMenu* menu);

	void update(double timeSinceLastFrame);

	void victory();
	void lose();

	
	//bool start(const CEGUI::EventArgs &e);
	bool pause(const CEGUI::EventArgs &e);
	bool resume(const CEGUI::EventArgs &e);
	bool backMenu(const CEGUI::EventArgs &e);
	bool exitGame(const CEGUI::EventArgs &e);
	bool nextGame(const CEGUI::EventArgs &e);
	bool loseGame(const CEGUI::EventArgs &e);

	CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
	{
		switch (buttonID)
		{
		case OIS::MB_Left:
			return CEGUI::LeftButton;

		case OIS::MB_Right:
			return CEGUI::RightButton;

		case OIS::MB_Middle:
			return CEGUI::MiddleButton;

		default:
			return CEGUI::LeftButton;
		}
	}

	void updateRedBall(int orient);
	void updateBlueBall(int orient);

private:
	Ogre::SceneNode*		m_pOgreHeadNode;
	Ogre::Entity*			m_pOgreHeadEntity;
	Ogre::MaterialPtr		m_pOgreHeadMat;
	Ogre::MaterialPtr		m_pOgreHeadMatHigh;

	OgreBites::ParamsPanel*		m_pDetailsPanel;
	bool				m_bQuit;

	Ogre::Vector3			m_TranslateVector;
	Ogre::Real			m_MoveSpeed;
	Ogre::Degree			m_RotateSpeed;
	float				m_MoveScale;
	Ogre::Degree			m_RotScale;

	Ogre::RaySceneQuery*		m_pRSQ;
	Ogre::SceneNode*		m_pCurrentObject;
	Ogre::Entity*			m_pCurrentEntity;
	bool				m_bLMouseDown, m_bRMouseDown;
	bool				m_bSettingsMode;



	double verDegree;	//垂直角度度数
	double horDegree;	//水平角度度数
	bool m_Pause;

	CEGUI::Window *startbgp;
	CEGUI::Window *mainpanel;
	CEGUI::Window *mainbgp;
	CEGUI::Window *pausebgp;
	CEGUI::Window *victorybgp;
	CEGUI::Window *losebgp;


	CEGUI::Window *skillPanel[5];

	CEGUI::Window *redballin;
	int redlevel;


	CEGUI::Window *blueballin;
	int bluelevel;

	//Ogre::AnimationState *mAnimationState;
	Ogre::SceneNode *camNode;

	Ogre::SceneNode * shotNode;
	Ogre::SceneNode * blowNode;


	// Physx 变量
	PxShape * hero_shape;
	PxRigidDynamic * hero_actor;
	PxShape * scene_shape;
	PxRigidStatic * scene_actor;
	PxController * heroController;

	//CEGUI::OgreRenderer* mRenderer;

	/* author by guoli*/
	Ogre::AnimationState *mAniStateWalk;
	Ogre::AnimationState *mAniStateFire;
	Ogre::AnimationState *mAniStateHit;

	Vector3 TargetPosition;
	EnemyManager * EnemyMgr;
	//SoundManager * SoundMgr;
	int HeroHP;
	enum HEROSTATE
	{
		NONE,
		WALK,
		ARCHER
	}HeroState;
	int wave[10];//每波敌人的数量
	int waveNum;//共有几波敌人
	int waveNow;//当前正在消灭哪一波敌人
	std::vector<Catapult *> CataSet;//投石车

	/* author by guoli*/
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||