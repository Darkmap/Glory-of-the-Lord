//|||||||||||||||||||||||||||||||||||||||||||||||

#include "GameState1.hpp"
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

//|||||||||||||||||||||||||||||||||||||||||||||||


extern SoundManager * SoundMgr;

#define PI 3.1415

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||


GameState1::GameState1()
{
	m_Pause = false;

	m_MoveSpeed		= 0.1f;
	m_RotateSpeed	= 0.3f;

	m_bLMouseDown       = false;
	m_bRMouseDown       = false;
	m_bQuit             = false;
	m_bSettingsMode     = false;

	m_pDetailsPanel	= 0;


	/*  Author by GL */
	//设定值
	TargetPosition=Vector3(0,-210,-780);
	waveNum=6;//预设共有5波敌人
	//预设每波敌人数量
	wave[0]=5;
	wave[1]=10;
	wave[2]=15;
	wave[3]=18;
	wave[4]=20;
	wave[5]=25;

	/*  Author by GL */
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState1::enter()
{
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering GameState1...");

	m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "GameSceneMgr");
	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

	m_pRSQ = m_pSceneMgr->createRayQuery(Ray());
	m_pRSQ->setQueryMask(OGRE_HEAD_MASK);

	m_pCamera = m_pSceneMgr->createCamera("GameCamera");
	m_pCamera->setPosition(Vector3(0, 60, 60));
	m_pCamera->lookAt(Vector3(0, 0, 0));
	m_pCamera->setNearClipDistance(5);

	m_pCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
		Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));

	OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
	m_pCurrentObject = 0;

	OgreFramework::getSingletonPtr()->m_pTrayMgr->hideCursor();
	//buildGUI();

	/*  Author by GL */
	// 初始化操作
	srand(time(0));//Ëæ»úÊýÖÖ×Ó
	waveNow=0;
	HeroHP=100;
	//SoundMgr->open("E:/TDDOWNLOAD/fight.wav","count");
	//SoundMgr->open("./count.wav","count");
	//SoundMgr->play("count",1);
	//SoundMgr->play("count",0);
	//SoundMgr->stop("sound1");

	//SoundMgr->play("fight(enemy)",0);
	m_Pause = false;

	EnemyMgr= new EnemyManager();
	/*  Author by GL */
	createScene();
}

bool GameState1::pause()
{
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Pausing GameState...");

	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(pausebgp);


	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();

	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState1::resume()
{
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Resuming GameState1...");

	buildGUI();

	OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
	m_bQuit = false;
}

void GameState1::exit()
{
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving GameState1...");

	m_pSceneMgr->destroyCamera(m_pCamera);
	m_pSceneMgr->destroyQuery(m_pRSQ);
	if(m_pSceneMgr)
		OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
	if(EnemyMgr)
		EnemyMgr->~EnemyManager();
	//if(SoundMgr)
		//SoundMgr->~SoundManager();
	CataSet.clear();
}

void GameState1::createScene()
{
	//创建一个天空盒（初始设置为蓝天白云）
	m_pSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8, 500);

	m_pSceneMgr->createLight("Light")->setPosition(-400,500,-600);

	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));
	
	////创建一个方向光源
 //	Ogre::Light* light = m_pSceneMgr->createLight("directionalLight");
 //	light->setType(Ogre::Light::LT_DIRECTIONAL);
 //	light->setDiffuseColour(Ogre::ColourValue(1, 1, 1));//白色光
 //	light->setSpecularColour(Ogre::ColourValue(1, 1, 1));
 //	light->setDirection(Ogre::Vector3( -1, -1, -1 )); 
 //	light->setVisible(true);


	//设置阴影
	m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	shotNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("shotNode");

	/// Ogre 创建英雄
	int scale = 2.3;
	m_pOgreHeadEntity = m_pSceneMgr->createEntity("Cube", "iceGirl.mesh");
	m_pOgreHeadEntity->setQueryFlags(OGRE_HEAD_MASK);
	m_pOgreHeadNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("CubeNode"/*,Ogre::Vector3(0,-10,-20)*/);
	m_pOgreHeadNode->scale(scale,scale,scale);
	m_pOgreHeadNode->attachObject(m_pOgreHeadEntity);
	m_pOgreHeadNode->setPosition(Vector3(0, 0, -25));
	m_pOgreHeadEntity->getMesh()->setAutoBuildEdgeLists(true);
	m_pOgreHeadEntity->setCastShadows(true);

	camNode = m_pOgreHeadNode->createChildSceneNode("CamNode",Ogre::Vector3(0,15,1));
	camNode->attachObject(m_pCamera);

	/***WQX：实现旋转角度限制---START***/
	verDegree = 0;
	horDegree = 0;
	/***WQX：实现旋转角度限制---END***/

	
	m_pOgreHeadNode->yaw(Ogre::Degree(180));
	//camNode->yaw(Ogre::Degree(180));

	//行走动画
	mAniStateWalk = m_pOgreHeadEntity->getAnimationState("font");
	//射击动画
	mAniStateFire = m_pOgreHeadEntity->getAnimationState("shoot");
	mAniStateFire->setLoop(false);
	//挨打动画
	mAniStateHit = m_pOgreHeadEntity->getAnimationState("attacked");
	mAniStateHit->setLoop(false);

	m_pOgreHeadMat = m_pOgreHeadEntity->getSubEntity(1)->getMaterial();
	m_pOgreHeadMatHigh = m_pOgreHeadMat->clone("OgreHeadMatHigh");
	m_pOgreHeadMatHigh->getTechnique(0)->getPass(0)->setAmbient(1, 0, 0);
	m_pOgreHeadMatHigh->getTechnique(0)->getPass(0)->setDiffuse(1, 0, 0, 0);

	/*  Author by JH */
	const AxisAlignedBox & box = m_pOgreHeadEntity->getBoundingBox();
	Vector3 size = box.getSize();
	float step_offset = 5;	
	// 创建 physx 中的英雄
	Vector3 position(0,size.y/2.0-200,-500);	
	//mphysx->createDynamicRigid(m_pOgreHeadEntity,scale,position,hero_actor,hero_shape);	
	mphysx->createCapsuleController(m_pOgreHeadEntity,0.5*scale,0.5*scale,position,step_offset,heroController);
	heroController->move(PxVec3(0,-5,0), 0.0001, 1/60, PxControllerFilters()); // 使英雄落地
	/*  Author by JH */


	/// Ogre 创建场景
	Entity * sceneEntity = m_pSceneMgr->createEntity("castle", "chengbao.mesh");
	SceneNode * sceneNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("castleNode");
	sceneNode->attachObject(sceneEntity);
	Quaternion q( Degree( 90 ), Vector3::UNIT_Y );
	sceneNode->rotate(q);
	sceneNode->setPosition(Vector3(0, 0, -25));
	sceneEntity->getMesh()->setAutoBuildEdgeLists(false);
	sceneEntity->setCastShadows(false);


	/*  Author by JH */
	// 创建 physx 中的场景
	Vector3 position1(0,0,-25);
	mphysx->createStaticRigid(sceneEntity,PI/2,position1,scene_actor,scene_shape);
	/*  Author by JH */

	/*  Author by GL */

	waveTimer.reset();//开始计时
	SoundMgr->play("count",1);

	CataSet.push_back(new Catapult(m_pSceneMgr,0,500,false));

	/*  Author by GL */



	/* CEGUI部分 */

	if(mRenderer==NULL)
		mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
	//mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();


	//	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
// 	CEGUI::SchemeManager::getSingleton().createFromFile("iconstarting.scheme");
// 	CEGUI::ImageManager::getSingleton().addFromImageFile("Cross", "cross.png");
// 	CEGUI::ImageManager::getSingleton().addFromImageFile("Bgp", "Starting.png");
// 	CEGUI::ImageManager::getSingleton().addFromImageFile("Cursor", "cursor.png");
// 	CEGUI::ImageManager::getSingleton().addFromImageFile("Panel1", "panel_full_1.png");
// 	CEGUI::ImageManager::getSingleton().addFromImageFile("Panel2", "panel_full_2.png");
// 	CEGUI::ImageManager::getSingleton().addFromImageFile("Panel3", "panel_full_3.png");
// 	CEGUI::ImageManager::getSingleton().addFromImageFile("Panel4", "panel_full_4.png");
// 	CEGUI::ImageManager::getSingleton().addFromImageFile("Panel5", "panel_full_5.png");

	//CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("Cursor");

	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();

	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	//CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");

	pausebgp = wmgr.createWindow("iconstarting/StaticImage", "Game/pausebgp");
	pausebgp->setSize(CEGUI::USize(cegui_reldim(1), cegui_reldim( 1)));
	pausebgp->setProperty("FrameEnabled", "false");
	pausebgp->setProperty("BackgroundEnabled", "false");
	pausebgp->setProperty("Image", "PauseBgp");

	CEGUI::Window *rsmGameButton = wmgr.createWindow("iconstarting/RsmGameButton", "Game/NewGameButton");
	rsmGameButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.370625, 0), CEGUI::UDim(0.35, 0)));
	rsmGameButton->setSize(CEGUI::USize(CEGUI::UDim(0.25875, 0), CEGUI::UDim(0.087525, 0)));
	pausebgp->addChild(rsmGameButton);
	rsmGameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameState1::resume, this));

	CEGUI::Window *bckGameButton = wmgr.createWindow("iconstarting/BckGameButton", "Game/BckGameButton");
	bckGameButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.370625, 0), CEGUI::UDim(0.47, 0)));
	bckGameButton->setSize(CEGUI::USize(CEGUI::UDim(0.25875, 0), CEGUI::UDim(0.087525, 0)));
	pausebgp->addChild(bckGameButton);
	bckGameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameState1::backMenu, this));


	CEGUI::Window *extGameButton = wmgr.createWindow("iconstarting/ExtGameButton", "Game/ExtGameButton");
	extGameButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.370625, 0), CEGUI::UDim(0.59, 0)));
	extGameButton->setSize(CEGUI::USize(CEGUI::UDim(0.25875, 0), CEGUI::UDim(0.087525, 0)));
	pausebgp->addChild(extGameButton);
	extGameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameState1::exitGame, this));


	/*

	startbgp = wmgr.createWindow("iconstarting/StaticImage", "Game/startbgp");
	startbgp->setSize(CEGUI::USize(cegui_reldim(1), cegui_reldim( 1)));
	startbgp->setProperty("FrameEnabled", "false");
	startbgp->setProperty("BackgroundEnabled", "false");
	startbgp->setProperty("Image", "Bgp");


	CEGUI::Window *newGameButton = wmgr.createWindow("iconstarting/NewGameButton", "Game/NewGameButton");
	newGameButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.370625, 0), CEGUI::UDim(0.35, 0)));
	newGameButton->setSize(CEGUI::USize(CEGUI::UDim(0.25875, 0), CEGUI::UDim(0.087525, 0)));
	startbgp->addChild(newGameButton);
	newGameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameState::start, this));

	CEGUI::Window *setGameButton = wmgr.createWindow("iconstarting/SetGameButton", "Game/SetGameButton");
	setGameButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.370625, 0), CEGUI::UDim(0.47, 0)));
	setGameButton->setSize(CEGUI::USize(CEGUI::UDim(0.25875, 0), CEGUI::UDim(0.087525, 0)));
	startbgp->addChild(setGameButton);


	CEGUI::Window *extGameButton = wmgr.createWindow("iconstarting/ExtGameButton", "Game/ExtGameButton");
	extGameButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.370625, 0), CEGUI::UDim(0.59, 0)));
	extGameButton->setSize(CEGUI::USize(CEGUI::UDim(0.25875, 0), CEGUI::UDim(0.087525, 0)));
	startbgp->addChild(extGameButton);

	///******暂时隐藏其他UI控件，调试
	*/

	mainpanel = wmgr.createWindow("DefaultWindow", "Game/mainwindow");
	
	CEGUI::Window *cross = wmgr.createWindow("iconstarting/StaticImage", "Game/Cross");
	cross->setPosition(CEGUI::UVector2(cegui_reldim(0.5-0.0375), cegui_reldim( 0.5-0.05)));
	cross->setSize(CEGUI::USize(cegui_reldim(0.075), cegui_reldim( 0.10)));
	cross->setProperty("FrameEnabled", "false");
	cross->setProperty("BackgroundEnabled", "false");
	cross->setProperty("Image", "Cross");
	mainpanel->addChild(cross);


	redballin = wmgr.createWindow("iconstarting/StaticImage", "Game/inBall");
	redballin->setPosition(CEGUI::UVector2(cegui_reldim(0.0157324), cegui_reldim(0.7625437)));
	redballin->setSize(CEGUI::USize(cegui_reldim(0.13225726), cegui_reldim( 0.1761315317)));
	redballin->setProperty("FrameEnabled", "false");
	redballin->setProperty("BackgroundEnabled", "false");
	redballin->setProperty("Image", "hpBallSet/100");
	mainpanel->addChild(redballin);
	redlevel = 100;

	blueballin = wmgr.createWindow("iconstarting/StaticImage", "Game/inBall2");
	blueballin->setPosition(CEGUI::UVector2(cegui_reldim(0.851), cegui_reldim(0.7625437)));
	blueballin->setSize(CEGUI::USize(cegui_reldim(0.13225726), cegui_reldim( 0.1761315317)));
	blueballin->setProperty("FrameEnabled", "false");
	blueballin->setProperty("BackgroundEnabled", "false");
	blueballin->setProperty("Image", "mpBallSet/100");
	mainpanel->addChild(blueballin);
	bluelevel = 100;

	mainbgp = wmgr.createWindow("iconstarting/StaticImage", "Game/Panel");
	mainbgp->setSize(CEGUI::USize(cegui_reldim(1), cegui_reldim( 1)));
	mainbgp->setProperty("FrameEnabled", "false");
	mainbgp->setProperty("BackgroundEnabled", "false");
	mainbgp->setProperty("Image", "Panel1");
	mainpanel->addChild(mainbgp);

	//*/
	skillPanel[0] = wmgr.createWindow("iconstarting/StaticImage", "Game/skill0");
	skillPanel[0]->setPosition(CEGUI::UVector2(cegui_reldim(0.329), cegui_reldim(0.90437494)));
	skillPanel[0]->setSize(CEGUI::USize(cegui_reldim(0.05754376), cegui_reldim( 0.07767)));
	skillPanel[0]->setProperty("FrameEnabled", "false");
	skillPanel[0]->setProperty("BackgroundEnabled", "false");
	skillPanel[0]->setProperty("Image", "Stop");
	skillPanel[0]->setAlpha(0.0);
	mainbgp->addChild(skillPanel[0]);

	skillPanel[1] = wmgr.createWindow("iconstarting/StaticImage", "Game/skill0");
	skillPanel[1]->setPosition(CEGUI::UVector2(cegui_reldim(0.401247), cegui_reldim(0.90437494)));
	skillPanel[1]->setSize(CEGUI::USize(cegui_reldim(0.05754376), cegui_reldim( 0.07767)));
	skillPanel[1]->setProperty("FrameEnabled", "false");
	skillPanel[1]->setProperty("BackgroundEnabled", "false");
	skillPanel[1]->setProperty("Image", "Stop");
	mainbgp->addChild(skillPanel[1]);

	skillPanel[2] = wmgr.createWindow("iconstarting/StaticImage", "Game/skill0");
	skillPanel[2]->setPosition(CEGUI::UVector2(cegui_reldim(0.47), cegui_reldim(0.90437494)));
	skillPanel[2]->setSize(CEGUI::USize(cegui_reldim(0.05754376), cegui_reldim( 0.07767)));
	skillPanel[2]->setProperty("FrameEnabled", "false");
	skillPanel[2]->setProperty("BackgroundEnabled", "false");
	skillPanel[2]->setProperty("Image", "Stop");
	mainbgp->addChild(skillPanel[2]);

	skillPanel[3] = wmgr.createWindow("iconstarting/StaticImage", "Game/skill0");
	skillPanel[3]->setPosition(CEGUI::UVector2(cegui_reldim(0.54149), cegui_reldim(0.90437494)));
	skillPanel[3]->setSize(CEGUI::USize(cegui_reldim(0.05754376), cegui_reldim( 0.07767)));
	skillPanel[3]->setProperty("FrameEnabled", "false");
	skillPanel[3]->setProperty("BackgroundEnabled", "false");
	skillPanel[3]->setProperty("Image", "Stop");
	mainbgp->addChild(skillPanel[3]);

	skillPanel[4] = wmgr.createWindow("iconstarting/StaticImage", "Game/skill0");
	skillPanel[4]->setPosition(CEGUI::UVector2(cegui_reldim(0.612), cegui_reldim(0.90437494)));
	skillPanel[4]->setSize(CEGUI::USize(cegui_reldim(0.05754376), cegui_reldim( 0.07767)));
	skillPanel[4]->setProperty("FrameEnabled", "false");
	skillPanel[4]->setProperty("BackgroundEnabled", "false");
	skillPanel[4]->setProperty("Image", "Stop");
	mainbgp->addChild(skillPanel[4]);

	//*/

	victorybgp = wmgr.createWindow("iconstarting/StaticImage", "Game/VictoryBgp");
	victorybgp->setSize(CEGUI::USize(cegui_reldim(1), cegui_reldim( 1)));
	victorybgp->setProperty("FrameEnabled", "false");
	victorybgp->setProperty("BackgroundEnabled", "false");
	victorybgp->setProperty("Image", "VictoryBgp");

	CEGUI::Window *nextButton = wmgr.createWindow("iconstarting/RsmGameButton", "Game/nextGameButton");
	nextButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.375, 0), CEGUI::UDim(0.603937, 0)));
	nextButton->setSize(CEGUI::USize(CEGUI::UDim(0.25144, 0), CEGUI::UDim(0.06811, 0)));
	victorybgp->addChild(nextButton);
	nextButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameState1::nextGame, this));


	losebgp = wmgr.createWindow("iconstarting/StaticImage", "Game/LoseBgp");
	losebgp->setSize(CEGUI::USize(cegui_reldim(1), cegui_reldim( 1)));
	losebgp->setProperty("FrameEnabled", "false");
	losebgp->setProperty("BackgroundEnabled", "false");
	losebgp->setProperty("Image", "LoseBgp");

	CEGUI::Window *loseButton = wmgr.createWindow("iconstarting/BckGameButton", "Game/loseBckGameButton");
	loseButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.375, 0), CEGUI::UDim(0.603937, 0)));
	loseButton->setSize(CEGUI::USize(CEGUI::UDim(0.25144, 0), CEGUI::UDim(0.06811, 0)));
	losebgp->addChild(loseButton);
	loseButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameState1::loseGame, this));


	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(mainpanel);
	/* CEGUI部分 */

}

bool GameState1::resume(const CEGUI::EventArgs &e)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(mainpanel);

	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();

	m_Pause = false;

	return true;
}

bool GameState1::backMenu(const CEGUI::EventArgs &e)
{

	m_Pause = false;
	pausebgp->hide();
	changeAppState(findByName("MenuState"));
	return true;
}

bool GameState1::exitGame(const CEGUI::EventArgs &e)
{
	m_bQuit = true;
	return true;
}


bool GameState1::nextGame(const CEGUI::EventArgs &e)
{
	changeAppState(findByName("GameState2"));
	return true;
}

bool GameState1::loseGame(const CEGUI::EventArgs &e)
{
	changeAppState(findByName("MenuState"));
	return true;
}

/*  Author by JH */
Vector3 rotateViewVector(float angle,Vector3 & v);

/*  Author by JH */



bool GameState1::keyPressed(const OIS::KeyEvent &keyEventRef)
{
// 	if(m_bSettingsMode == true)
// 	{
// 		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_S))
// 		{
// 			OgreBites::SelectMenu* pMenu = (OgreBites::SelectMenu*)OgreFramework::getSingletonPtr()->m_pTrayMgr->getWidget("ChatModeSelMenu");
// 			if(pMenu->getSelectionIndex() + 1 < (int)pMenu->getNumItems())
// 				pMenu->selectItem(pMenu->getSelectionIndex() + 1);
// 		}
// 
// 		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_W))
// 		{
// 			OgreBites::SelectMenu* pMenu = (OgreBites::SelectMenu*)OgreFramework::getSingletonPtr()->m_pTrayMgr->getWidget("ChatModeSelMenu");
// 			if(pMenu->getSelectionIndex() - 1 >= 0)
// 				pMenu->selectItem(pMenu->getSelectionIndex() - 1);
// 		}
// 	}

	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
	{
		m_Pause = true;
		pause();
		return true;
	}
	
// 	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_I))
// 	{
// 		if(m_pDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
// 		{
// 			OgreFramework::getSingletonPtr()->m_pTrayMgr->moveWidgetToTray(m_pDetailsPanel, OgreBites::TL_TOPLEFT, 0);
// 			m_pDetailsPanel->show();
// 		}
// 		else
// 		{
// 			OgreFramework::getSingletonPtr()->m_pTrayMgr->removeWidgetFromTray(m_pDetailsPanel);
// 			m_pDetailsPanel->hide();
// 		}
// 	}

	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_TAB))
	{
		m_bSettingsMode = !m_bSettingsMode;
		return true;
	}

	if(m_bSettingsMode && OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_RETURN) ||
		OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_NUMPADENTER))
	{
	}

	if(!m_bSettingsMode || (m_bSettingsMode && !OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_O)))
		OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);


	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	context.injectKeyDown((CEGUI::Key::Scan)keyEventRef.key);
	context.injectChar((CEGUI::Key::Scan)keyEventRef.text);

	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState1::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);

	/*  Author by GL */
	if (keyEventRef.key)
	{
	    HeroState=NONE;
		mAniStateWalk = m_pOgreHeadEntity->getAnimationState("font");
		mAniStateWalk->setEnabled(false);
		//mAniStateWalk->setTimePosition(0.0);
		mAniStateWalk = m_pOgreHeadEntity->getAnimationState("back");
		//mAniStateWalk->setTimePosition(0.0);
		mAniStateWalk->setEnabled(false);
		mAniStateWalk = m_pOgreHeadEntity->getAnimationState("left");
		//mAniStateWalk->setTimePosition(0.0);
		mAniStateWalk->setEnabled(false);
		mAniStateWalk = m_pOgreHeadEntity->getAnimationState("right");
		//mAniStateWalk->setTimePosition(0.0);
		mAniStateWalk->setEnabled(false);
	}
	/*  Author by GL */
/*  Author by JH */
#if 1
	PxVec3 v(0,0,0);
	switch (keyEventRef.key)
	{
	case OIS::KC_UP:
		updateRedBall(1);
		break;
	case OIS::KC_DOWN:
		updateRedBall(-1);
		break;
	case OIS::KC_LEFT:
		updateBlueBall(1);
		break;
	case OIS::KC_RIGHT:
		updateBlueBall(-1);
		break;
	case OIS::KC_1:
		mainbgp->setProperty("Image", "Panel1");
		break;
	/*case OIS::KC_2:
		mainbgp->setProperty("Image", "Panel2");
		break;
	case OIS::KC_3:
		mainbgp->setProperty("Image", "Panel3");
		break;
	case OIS::KC_4:
		mainbgp->setProperty("Image", "Panel4");
		break;
	case OIS::KC_5:
		mainbgp->setProperty("Image", "Panel5");
		break;*/
	////////////////////////////////////测试-直升第二关
	case OIS::KC_0:
		changeAppState(findByName("GameState2"));
		return true;
		////////////////////////////////////////////
	}
#endif
/*  Author by JH */

	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)keyEventRef.key);
	return true;
}

void GameState1::updateBlueBall(int orient){

	if(bluelevel>0 && orient==-1)
		bluelevel -= 5;
	if(bluelevel<100 && orient==1)
		bluelevel += 5;

	switch (bluelevel)
	{
	case 0:
		blueballin->setProperty("Image","");
		break;
	case 5:
		blueballin->setProperty("Image","mpBallSet/5");
		break;
	case 10:
		blueballin->setProperty("Image","mpBallSet/10");
		break;
	case 15:
		blueballin->setProperty("Image","mpBallSet/15");
		break;
	case 20:
		blueballin->setProperty("Image","mpBallSet/20");
		break;
	case 25:
		blueballin->setProperty("Image","mpBallSet/25");
		break;
	case 30:
		blueballin->setProperty("Image","mpBallSet/30");
		break;
	case 35:
		blueballin->setProperty("Image","mpBallSet/35");
		break;
	case 40:
		blueballin->setProperty("Image","mpBallSet/40");
		break;
	case 45:
		blueballin->setProperty("Image","mpBallSet/45");
		break;
	case 50:
		blueballin->setProperty("Image","mpBallSet/50");
		break;
	case 55:
		blueballin->setProperty("Image","mpBallSet/55");
		break;
	case 60:
		blueballin->setProperty("Image","mpBallSet/60");
		break;
	case 65:
		blueballin->setProperty("Image","mpBallSet/65");
		break;
	case 70:
		blueballin->setProperty("Image","mpBallSet/70");
		break;
	case 75:
		blueballin->setProperty("Image","mpBallSet/75");
		break;
	case 80:
		blueballin->setProperty("Image","mpBallSet/80");
		break;
	case 85:
		blueballin->setProperty("Image","mpBallSet/85");
		break;
	case 90:
		blueballin->setProperty("Image","mpBallSet/90");
		break;
	case 95:
		blueballin->setProperty("Image","mpBallSet/95");
		break;
	case 100:
		blueballin->setProperty("Image","mpBallSet/100");
		break;
	}
}
void GameState1::updateRedBall(int orient){

	if(redlevel>0 && orient==-1)
		redlevel -= 5;
	if(redlevel<100 && orient==1)
		redlevel += 5;

	switch (redlevel)
	{
	case 0:
		redballin->setProperty("Image","");
		break;
	case 5:
		redballin->setProperty("Image","hpBallSet/5");
		break;
	case 10:
		redballin->setProperty("Image","hpBallSet/10");
		break;
	case 15:
		redballin->setProperty("Image","hpBallSet/15");
		break;
	case 20:
		redballin->setProperty("Image","hpBallSet/20");
		break;
	case 25:
		redballin->setProperty("Image","hpBallSet/25");
		break;
	case 30:
		redballin->setProperty("Image","hpBallSet/30");
		break;
	case 35:
		redballin->setProperty("Image","hpBallSet/35");
		break;
	case 40:
		redballin->setProperty("Image","hpBallSet/40");
		break;
	case 45:
		redballin->setProperty("Image","hpBallSet/45");
		break;
	case 50:
		redballin->setProperty("Image","hpBallSet/50");
		break;
	case 55:
		redballin->setProperty("Image","hpBallSet/55");
		break;
	case 60:
		redballin->setProperty("Image","hpBallSet/60");
		break;
	case 65:
		redballin->setProperty("Image","hpBallSet/65");
		break;
	case 70:
		redballin->setProperty("Image","hpBallSet/70");
		break;
	case 75:
		redballin->setProperty("Image","hpBallSet/75");
		break;
	case 80:
		redballin->setProperty("Image","hpBallSet/80");
		break;
	case 85:
		redballin->setProperty("Image","hpBallSet/85");
		break;
	case 90:
		redballin->setProperty("Image","hpBallSet/90");
		break;
	case 95:
		redballin->setProperty("Image","hpBallSet/95");
		break;
	case 100:
		redballin->setProperty("Image","hpBallSet/100");
		break;
	}
}


bool GameState1::mouseMoved(const OIS::MouseEvent &evt)
{
	//if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;

// 	if(m_bRMouseDown)
// 	{
// // 		m_pCamera->yaw(Degree(evt.state.X.rel * -0.1f));
// // 		m_pCamera->pitch(Degree(evt.state.Y.rel * -0.1f));
// 		m_pOgreHeadNode->yaw(Degree(evt.state.X.rel * -0.1f));
// 		m_pCamera->pitch(Degree(evt.state.Y.rel * -0.1f));
// 	}


	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.getDefaultGUIContext().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);
	// Scroll wheel.
	if (evt.state.Z.rel)
		sys.getDefaultGUIContext().injectMouseWheelChange(evt.state.Z.rel / 120.0f);



	/***
	**暂停状态下不再刷新以下内容*
	***/

	if(m_Pause)
		return true;

	horDegree = horDegree + (evt.state.X.rel * -0.4f);
	m_pOgreHeadNode->yaw(Degree(evt.state.X.rel * -0.4f));


	if(verDegree>80){
		if((evt.state.Y.rel * -0.2f)<0){
			verDegree = verDegree + (evt.state.Y.rel * -0.2f);
			camNode->pitch(Degree(evt.state.Y.rel * -0.2f),Ogre::Node::TS_PARENT);
		}
	}
	else if(verDegree>=-45){
		verDegree = verDegree + (evt.state.Y.rel * -0.2f);
		camNode->pitch(Degree(evt.state.Y.rel * -0.2f),Ogre::Node::TS_PARENT);
	}
	else if(verDegree<-45){
		if((evt.state.Y.rel * -0.2f)>0){
			verDegree = verDegree + (evt.state.Y.rel * -0.2f);
			camNode->pitch(Degree(evt.state.Y.rel * -0.2f),Ogre::Node::TS_PARENT);
		}
	}
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState1::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	//if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;

	if(id == OIS::MB_Left)
	{
		onLeftPressed(evt);
		m_bLMouseDown = true;
	}
	else if(id == OIS::MB_Right)
	{
		m_bRMouseDown = true;
	}

	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));

	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState1::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	//if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;

	if(id == OIS::MB_Left)
	{
		m_bLMouseDown = false;

		shotNode->_setDerivedOrientation(m_pOgreHeadNode->_getDerivedOrientation());
		shotNode->_setDerivedPosition(m_pOgreHeadNode->_getDerivedPosition());
		shotNode->attachObject(ParticleSystemTools::getShot(m_pSceneMgr));
	}
	else if(id == OIS::MB_Right)
	{
		m_bRMouseDown = false;
	}

	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
	return true;
}

void GameState1::onLeftPressed(const OIS::MouseEvent &evt)
{
// 	if(m_pCurrentObject)
// 	{
// 		m_pCurrentObject->showBoundingBox(false);
// 		m_pCurrentEntity->getSubEntity(1)->setMaterial(m_pOgreHeadMat);
// 	}
// 
// 	Ogre::Ray mouseRay = m_pCamera->getCameraToViewportRay(OgreFramework::getSingletonPtr()->m_pMouse->getMouseState().X.abs / float(evt.state.width),
// 		OgreFramework::getSingletonPtr()->m_pMouse->getMouseState().Y.abs / float(evt.state.height));
// 	m_pRSQ->setRay(mouseRay);
// 	m_pRSQ->setSortByDistance(true);
// 
// 	Ogre::RaySceneQueryResult &result = m_pRSQ->execute();
// 	Ogre::RaySceneQueryResult::iterator itr;
// 
// 	for(itr = result.begin(); itr != result.end(); itr++)
// 	{
// 		if(itr->movable)
// 		{
// 			OgreFramework::getSingletonPtr()->m_pLog->logMessage("MovableName: " + itr->movable->getName());
// 			m_pCurrentObject = m_pSceneMgr->getEntity(itr->movable->getName())->getParentSceneNode();
// 			OgreFramework::getSingletonPtr()->m_pLog->logMessage("ObjName " + m_pCurrentObject->getName());
// 			m_pCurrentObject->showBoundingBox(true);
// 			m_pCurrentEntity = m_pSceneMgr->getEntity(itr->movable->getName());
// 			m_pCurrentEntity->getSubEntity(1)->setMaterial(m_pOgreHeadMatHigh);
// 			break;
// 		}
// 	}

	
	if(m_Pause){
		return;
	}

	/*  Author by GL */
	//不论是否打到敌人，射击就播放动画
 	mAniStateFire->setEnabled(true);
	SoundMgr->play("arrow",1);

	//判断是否打中
 	Ogre::Ray thumbRay = m_pCamera->getCameraToViewportRay(0.5,0.5);
 	m_pRSQ->setRay(thumbRay);
 	m_pRSQ->setSortByDistance(true);
 
 	Ogre::RaySceneQueryResult &result = m_pRSQ->execute();
 	Ogre::RaySceneQueryResult::iterator itr;
 
 	for(itr = result.begin(); itr != result.end(); itr++)
 	{
		if(itr->movable && itr->distance>0)   // 排除摄像机
 		{
			//判断是敌人，Enemy
			if (itr->movable->getName()[0]=='E')
			{				
				// 释放OGRE实体
				int index = EnemyMgr->findByName(itr->movable->getName());
				//m_pCurrentObject->detachObject(m_pCurrentEntity);
				if(index!=-1&& EnemyMgr->enemySet[index]->state!=3)
				{
					//EnemyMgr->enemySet[index]->scenenode->detachAllObjects();
					//EnemyMgr->enemySet[index]->controller->release();
					EnemyMgr->enemySet[index]->state=Enemy::HIT;//HIT;
					EnemyMgr->enemySet[index]->hitAnimation->setEnabled(true);
					EnemyMgr->enemySet[index]->hitAnimation->setTimePosition(0.0f);
					EnemyMgr->enemySet[index]->hp-=5;
					if (EnemyMgr->enemySet[index]->hp<=0)
					{
						EnemyMgr->enemySet[index]->state=Enemy::WILLDEAD;//确定会死亡，WILLDEAD
						EnemyMgr->enemySet[index]->fallAnimation->setEnabled(true);
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
			break;
 		}
 	}
	/*  Author by GL */
}

void GameState1::moveCamera(double timeSinceLastFrame)
{
// 	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
// 		m_pCamera->moveRelative(m_TranslateVector);
// 	m_pCamera->moveRelative(m_TranslateVector / 10);
	
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
		m_pOgreHeadNode->translate(m_TranslateVector * 2 , Ogre::Node::TS_LOCAL);
	m_pOgreHeadNode->translate(m_TranslateVector, Ogre::Node::TS_LOCAL);
// 
// 	if(!m_TranslateVector.isZeroLength())
// 		mAnimationState->addTime(timeSinceLastFrame/1000.0);

//	m_pOgreHeadNode->translate(m_TranslateVector,Ogre::Node::TS_LOCAL);
}

void GameState1::getInput(double timeSinceLastFrame)
{
	float gravity = -5;
	if(m_bSettingsMode == false)
	{
/*  Author by JH */		
		float step = 50*timeSinceLastFrame/1000.0;  // 用来调整速度
		Vector3 orientation;
		orientation.x = m_pOgreHeadNode->_getDerivedPosition().x - camNode->_getDerivedPosition().x;
		orientation.y = 0;
		orientation.z = m_pOgreHeadNode->_getDerivedPosition().z - camNode->_getDerivedPosition().z;
		orientation.normalise();
		Vector3 x_orientation = rotateViewVector(-90,orientation);  /// 指向局部坐标 x 轴正方向
		PxVec3 v(0,0,0);
			
		/// 控制英雄行走
		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_A))
		{
			v.x = -step*x_orientation.x;
			v.y = -step*x_orientation.y+gravity;
			v.z = -step*x_orientation.z;
			//hero_actor->setLinearVelocity(v);
			heroController->move(PxVec3(v.x,v.y,v.z), 0.0001, 1/60, PxControllerFilters());
			/*  Author by GL */
			mAniStateWalk = m_pOgreHeadEntity->getAnimationState("left");
			mAniStateWalk->setLoop(false);
			HeroState=WALK;
			/*  Author by GL */
		}
		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_D))
		{
			v.x = step*x_orientation.x;
			v.y = step*x_orientation.y+gravity;
			v.z = step*x_orientation.z;
			//hero_actor->setLinearVelocity(v);
			heroController->move(PxVec3(v.x,v.y,v.z), 0.0001, 1/60, PxControllerFilters());	

			/*  Author by GL */
			mAniStateWalk = m_pOgreHeadEntity->getAnimationState("right");
			mAniStateWalk->setLoop(false);
			HeroState=WALK;
			/*  Author by GL */
		}

		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_W))
		{
			v.x = step*orientation.x;
			v.y = step*orientation.y+gravity;
			v.z = step*orientation.z;
			//hero_actor->setLinearVelocity(v);
			heroController->move(PxVec3(v.x,v.y,v.z), 0.0001, 1/60, PxControllerFilters());	

			/*  Author by GL */
			if (mAniStateWalk->getAnimationName()!="font")
				mAniStateWalk->setTimePosition(0.0);
			mAniStateWalk = m_pOgreHeadEntity->getAnimationState("font");
			mAniStateWalk->setLoop(false);
			HeroState=WALK;
			/*  Author by GL */
		}

		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_S))
		{
			v.x = -step*orientation.x;
			v.y = -step*orientation.y+gravity;
			v.z = -step*orientation.z;
			//hero_actor->setLinearVelocity(v);
			heroController->move(PxVec3(v.x,v.y,v.z), 0.0001, 1/60, PxControllerFilters());	

			/*  Author by GL */
			mAniStateWalk = m_pOgreHeadEntity->getAnimationState("back");
			mAniStateWalk->setLoop(false);
			HeroState=WALK;
			/*  Author by GL */
		}

/*  Author by JH */

		/*  Author by GL */	
		if(HeroState==WALK)
		{
			mAniStateWalk->setEnabled(true);
			if(mAniStateWalk->hasEnded())
			{
				mAniStateWalk->setTimePosition(0.0f);
			}
		}
		else
		{
			mAniStateWalk->setTimePosition(0.0f);
			mAniStateWalk->setEnabled(false);
		}
		/*  Author by GL */



		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_Q))
			m_TranslateVector.y = -m_MoveScale;

		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_E))
			m_TranslateVector.y = m_MoveScale;

		//camera roll
		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_Z))
			m_pCamera->roll(Angle(-m_MoveScale));

		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_X))
			m_pCamera->roll(Angle(m_MoveScale));

		//reset roll
		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_C))
			m_pCamera->roll(-(m_pCamera->getRealOrientation().getRoll()));
	}
}

void GameState1::update(double timeSinceLastFrame)
{
	if(m_bQuit == true)
	{
		popAppState();
		return;
	}

	if(m_Pause)
		return;

	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;

 //	if(!OgreFramework::getSingletonPtr()->m_pTrayMgr->isDialogVisible())
 //	{
 //		if(m_pDetailsPanel->isVisible())
 //		{
	//		char numCh[10];
	//		itoa(EnemyMgr->defenseNum,numCh,10);
	//		m_pDetailsPanel->setParamValue(0, numCh);

	//		itoa(EnemyMgr->aliveNum,numCh,10);
	//		m_pDetailsPanel->setParamValue(1, numCh);

	//	/*	m_pDetailsPanel->setParamValue(0, EnemyMgr->aliveNum);
 //			m_pDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(m_pCamera->getDerivedPosition().y));
 //			m_pDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(m_pCamera->getDerivedPosition().z));
 //			m_pDetailsPanel->setParamValue(3, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().w));
 //			m_pDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().x));
 //			m_pDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().y));
 //			m_pDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().z));*/
 //// 			m_pDetailsPanel->setParamValue(3, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().w));
 //// 			m_pDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().x));
 //// 			m_pDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().y));
 //// 			m_pDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().z));
 //			/*if(m_bSettingsMode)
 //				m_pDetailsPanel->setParamValue(7, "Buffered Input");
 //			else
 //				m_pDetailsPanel->setParamValue(7, "Un-Buffered Input");*/
 //		}
 //	}

	m_MoveScale = m_MoveSpeed   * timeSinceLastFrame;
	m_RotScale  = m_RotateSpeed * timeSinceLastFrame;

	m_TranslateVector = Vector3::ZERO;

	//mAnimationState->addTime(timeSinceLastFrame/1000.0);

	/*  Author by GL */
  	mAniStateWalk->addTime(timeSinceLastFrame/1000.0);//英雄行走动画
	mAniStateFire->addTime(timeSinceLastFrame/1000.0);//英雄射箭动画
	mAniStateHit->addTime(timeSinceLastFrame/1000.0);//受到攻击和死亡

	EnemyMgr->update(TargetPosition,timeSinceLastFrame);//更新敌人状态
	
	//更新投石车
	for (int i=0;i<CataSet.size();i++)
	{
		if (CataSet[i]->update(m_pOgreHeadNode->getPosition(),timeSinceLastFrame))//被攻击
		{
			HeroHP-=CataSet[i]->damage;//更新英雄血量
			mAniStateHit->setEnabled(true);
		}
	}
	//关联英雄血量到红球
	redlevel = HeroHP;
	updateRedBall(0);
	
	if (HeroHP<=0 || m_pOgreHeadNode->getPosition().y<-500)//英雄死了
	{
		//播放死亡动画
		mAniStateHit = m_pOgreHeadEntity->getAnimationState("die");
		mAniStateHit->setLoop(false);
		mAniStateHit->setEnabled(true);
		if (mAniStateHit->hasEnded())
		{
			//gameover
			SoundMgr->stop("fight");
			lose();
			return;
		}
	}

	if (EnemyMgr->aliveNum==0 && waveTimer.getMilliseconds()>=3000)//
	{
		if (waveNow<waveNum)
		{
			EnemyMgr->createEnemy(1,m_pSceneMgr,0,500,70,wave[waveNow]);
			if (waveNow==1)
			{
				CataSet.push_back(new Catapult(m_pSceneMgr,-150,500,true));//放置第二个投石车，可跟踪英雄
			}
			if (waveNow==3)
			{
				CataSet.push_back(new Catapult(m_pSceneMgr,150,500,false));//放置第三个投石车，不跟踪
			}
			waveNow++;
			SoundMgr->play("fight",0);
		}
	}
	if (EnemyMgr->aliveNum==0 && waveNow>=waveNum)
	{
		//victory;!!!!!!!!!!!!!!!!!!!!		
		SoundMgr->stop("fight");
		SoundMgr->stop("count");
		victory();
		return;

	}
	if (EnemyMgr->defenseNum<=0)//失守
	{
		//gameover!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		SoundMgr->stop("fight");
		lose();
		return;
	}

	//更新闯入敌人数量状态
	bluelevel = EnemyMgr -> defenseNum * 10;
	updateBlueBall(0);

	if(mAniStateFire->hasEnded())
	{
		mAniStateFire->setTimePosition(0.0f);
		mAniStateFire->setEnabled(false);
	}
	if(mAniStateHit->hasEnded())
	{
		mAniStateHit->setTimePosition(0.0f);
		mAniStateHit->setEnabled(false);
	}

	/*  Author by GL */

	getInput(timeSinceLastFrame);
	moveCamera(timeSinceLastFrame);


/*  Author by JH */
	/// Ogre 计算方向和得到 Quaternion
	/*Quaternion q = m_pOgreHeadNode->_getDerivedOrientation();

	/// 由 Physx 改变位置，并且改变英雄的 方向(Quaternion)
	PxTransform boxPosition = PxShapeExt::getGlobalPose(*hero_shape);	
	boxPosition.q.x = q.x;
	boxPosition.q.y = q.y;
	boxPosition.q.z = q.z;
	boxPosition.q.w = q.w;
	hero_actor->setGlobalPose(boxPosition);

	/// Ogre 获得位置并且更新
	m_pOgreHeadNode->setPosition(boxPosition.p.x,boxPosition.p.y,boxPosition.p.z);
	mphysx->start_simulate(1.0f / 60.0f);*/

	PxExtendedVec3 heroposition = heroController->getPosition();
	m_pOgreHeadNode->setPosition(heroposition.x,heroposition.y-9,heroposition.z);
	mphysx->start_simulate(1.0f / 60.0f);

/*  Author by JH */




	CEGUI::System::getSingleton().injectTimePulse(timeSinceLastFrame/1000.0);



//  	m_pOgreHeadNode->setPosition(m_pCamera->getDerivedPosition().x,m_pCamera->getDerivedPosition().y,m_pCamera->getDerivedPosition().z+100);
//  	m_pOgreHeadNode->setOrientation(m_pCamera->getDerivedOrientation());

}

void GameState1::buildGUI()
{
 	/*OgreFramework::getSingletonPtr()->m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
 	OgreFramework::getSingletonPtr()->m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
 	OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_TOP, "GameLbl", "Game mode", 250);
 	OgreFramework::getSingletonPtr()->m_pTrayMgr->showCursor();
 
 	Ogre::StringVector items;
 	items.push_back("cam.pX");
 	items.push_back("cam.pY");
 	items.push_back("cam.pZ");
 	items.push_back("cam.oW");
 	items.push_back("cam.oX");
 	items.push_back("cam.oY");
 	items.push_back("cam.oZ");
 	items.push_back("Mode");
 
 	m_pDetailsPanel = OgreFramework::getSingletonPtr()->m_pTrayMgr->createParamsPanel(OgreBites::TL_TOPLEFT, "DetailsPanel", 200, items);
 	m_pDetailsPanel->show();
 */
 	//Ogre::String infoText = "[TAB] - Switch input mode\n\n[W] - Forward / Mode up\n[S] - Backwards/ Mode down\n[A] - Left\n";
 	//infoText.append("[D] - Right\n\nPress [SHIFT] to move faster\n\n[O] - Toggle FPS / logo\n");
 	//infoText.append("[Print] - Take screenshot\n\n[ESC] - Exit");
 	//OgreFramework::getSingletonPtr()->m_pTrayMgr->createTextBox(OgreBites::TL_RIGHT, "InfoPanel", infoText, 300, 220);
 
 	//Ogre::StringVector chatModes;
 	//chatModes.push_back("Solid mode");
 	//chatModes.push_back("Wireframe mode");
 	//chatModes.push_back("Point mode");
 	//OgreFramework::getSingletonPtr()->m_pTrayMgr->createLongSelectMenu(OgreBites::TL_TOPRIGHT, "ChatModeSelMenu", "ChatMode", 200, 3, chatModes);
}

void GameState1::itemSelected(OgreBites::SelectMenu* menu)
{
	switch(menu->getSelectionIndex())
	{
	case 0:
		m_pCamera->setPolygonMode(Ogre::PM_SOLID);break;
	case 1:
		m_pCamera->setPolygonMode(Ogre::PM_WIREFRAME);break;
	case 2:
		m_pCamera->setPolygonMode(Ogre::PM_POINTS);break;
	}
}




void GameState1::victory(){
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(victorybgp);
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
	m_Pause = true;
}

void GameState1::lose(){
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(losebgp);
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
	m_Pause = true;
}