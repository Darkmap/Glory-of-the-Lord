//|||||||||||||||||||||||||||||||||||||||||||||||

#include "MenuState.hpp"
#include "SoundManager.h"
#include <streams.h>
//#include "windows.h"
//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;
extern SoundManager * SoundMgr;


//|||||||||||||||||||||||||||||||||||||||||||||||

MenuState::MenuState()
{
	m_bQuit         = false;
	m_FrameEvent    = Ogre::FrameEvent();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void MenuState::enter()
{
	SoundMgr->play("mainMenu",0);
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering MenuState...");

	m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "MenuSceneMgr");
	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

	m_pCamera = m_pSceneMgr->createCamera("MenuCam");
	m_pCamera->setPosition(Vector3(0, 25, -50));
	m_pCamera->lookAt(Vector3(0, 0, 0));
	m_pCamera->setNearClipDistance(1);

	m_pCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
		Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));

	OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);

// 	OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
// 	OgreFramework::getSingletonPtr()->m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
// 	OgreFramework::getSingletonPtr()->m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
// 	OgreFramework::getSingletonPtr()->m_pTrayMgr->showCursor();
// 	OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "EnterBtn", "Enter GameState", 250);
// 	OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "ExitBtn", "Exit AdvancedOgreFramework", 250);
// 	OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_TOP, "MenuLbl", "Menu mode", 250);

	createScene();
}

void MenuState::createScene()
{
	if(mRenderer==NULL){
		mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
		CEGUI::SchemeManager::getSingleton().createFromFile("iconstarting.scheme");
		CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
		CEGUI::SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
		CEGUI::ImageManager::getSingleton().addFromImageFile("Cross", "cross.png");
		CEGUI::ImageManager::getSingleton().addFromImageFile("Stop", "cancel.png");
		CEGUI::ImageManager::getSingleton().addFromImageFile("MenuBgp", "Starting.png");
		CEGUI::ImageManager::getSingleton().addFromImageFile("SettingBgp", "Setting.png");
		CEGUI::ImageManager::getSingleton().addFromImageFile("PauseBgp", "pause.png");
		CEGUI::ImageManager::getSingleton().addFromImageFile("VictoryBgp", "victory.png");
		CEGUI::ImageManager::getSingleton().addFromImageFile("LoseBgp", "lose.png");
		CEGUI::ImageManager::getSingleton().addFromImageFile("Cursor", "cursor.png");
		CEGUI::ImageManager::getSingleton().addFromImageFile("Panel1", "panel_full_1.png");
		CEGUI::ImageManager::getSingleton().addFromImageFile("Panel2", "panel_full_2.png");
		CEGUI::ImageManager::getSingleton().addFromImageFile("Panel3", "panel_full_3.png");
		CEGUI::ImageManager::getSingleton().addFromImageFile("Panel4", "panel_full_4.png");
		CEGUI::ImageManager::getSingleton().addFromImageFile("Panel5", "panel_full_5.png");
		CEGUI::ImageManager::getSingleton().loadImageset("hpBallSet.imageset");
		CEGUI::ImageManager::getSingleton().loadImageset("mpBallSet.imageset");
		
	}

	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("Cursor");

	if(!CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().isVisible())
		CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();

	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	//CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");

	startbgp = wmgr.createWindow("iconstarting/StaticImage", "Game/startbgp");
	startbgp->setSize(CEGUI::USize(cegui_reldim(1), cegui_reldim( 1)));
	startbgp->setProperty("FrameEnabled", "false");
	startbgp->setProperty("BackgroundEnabled", "false");
	startbgp->setProperty("Image", "MenuBgp");


	CEGUI::Window *newGameButton = wmgr.createWindow("iconstarting/NewGameButton", "Game/NewGameButton");
	newGameButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.370625, 0), CEGUI::UDim(0.35, 0)));
	newGameButton->setSize(CEGUI::USize(CEGUI::UDim(0.25875, 0), CEGUI::UDim(0.087525, 0)));
	startbgp->addChild(newGameButton);
	newGameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MenuState::start, this));

	CEGUI::Window *setGameButton = wmgr.createWindow("iconstarting/SetGameButton", "Game/SetGameButton");
	setGameButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.370625, 0), CEGUI::UDim(0.47, 0)));
	setGameButton->setSize(CEGUI::USize(CEGUI::UDim(0.25875, 0), CEGUI::UDim(0.087525, 0)));
	startbgp->addChild(setGameButton);
	setGameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MenuState::openSetting, this));


	CEGUI::Window *extGameButton = wmgr.createWindow("iconstarting/ExtGameButton", "Game/setExtGameButton");
	extGameButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.370625, 0), CEGUI::UDim(0.59, 0)));
	extGameButton->setSize(CEGUI::USize(CEGUI::UDim(0.25875, 0), CEGUI::UDim(0.087525, 0)));
	startbgp->addChild(extGameButton);
	extGameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MenuState::exitGame, this));


	setbgp = wmgr.createWindow("iconstarting/StaticImage", "Game/settingbgp");
	setbgp->setSize(CEGUI::USize(cegui_reldim(1), cegui_reldim( 1)));
	setbgp->setProperty("FrameEnabled", "false");
	setbgp->setProperty("BackgroundEnabled", "false");
	setbgp->setProperty("Image", "SettingBgp");

	openSoundCheckBox = (CEGUI::ToggleButton*)wmgr.createWindow("Vanilla/Checkbox", "Game/SoundCheckbox");
	openSoundCheckBox->setArea(CEGUI::UDim(0.411487, 0),CEGUI::UDim(0.318898, 0),CEGUI::UDim(0.04385, 0),CEGUI::UDim(0.05846, 0));
	setbgp->addChild(openSoundCheckBox);
	openSoundCheckBox->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&MenuState::soundIf,this));
	openSoundCheckBox->setSelected(true);


	CEGUI::Window *setOkButton = wmgr.createWindow("iconstarting/OkGameButton", "Game/OkGameButton");
	setOkButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.36926, 0), CEGUI::UDim(0.7116, 0)));
	setOkButton->setSize(CEGUI::USize(CEGUI::UDim(0.311974, 0), CEGUI::UDim(0.07933, 0)));
	setbgp->addChild(setOkButton);
	setOkButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MenuState::setComplete, this));

	CEGUI::Scrollbar *soundDegreeBar = (CEGUI::Scrollbar *)wmgr.createWindow("Vanilla/HorizontalScrollbar", "Game/HorizontalScrollbar");
	setbgp->addChild(soundDegreeBar);
	soundDegreeBar->setPosition(CEGUI::UVector2(CEGUI::UDim(0.411487, 0), CEGUI::UDim(0.412, 0)));
	soundDegreeBar->setSize(CEGUI::USize(CEGUI::UDim(0.33663, 0), CEGUI::UDim(0.022, 0)));
	soundDegreeBar->setScrollPosition(0.5);
	soundDegreeBar->subscribeEvent(CEGUI::Scrollbar::EventScrollPositionChanged, CEGUI::Event::Subscriber(&MenuState::soundScroll, this));

	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(startbgp);
}

//|||||||||||||||||||||||||||||||||||||||||||||||
bool MenuState::soundScroll(const CEGUI::EventArgs &e)
{
	//SoundMgr->stop("mainMenu");
	
	float soundValue = ((CEGUI::Scrollbar*)(static_cast<const CEGUI::WindowEventArgs&>(e).window))->getScrollPosition();

	SoundMgr->attenuation = -(1.0-soundValue)*10000;

	SoundMgr->setVolume();

	//SoundMgr->play("mainMenu",0);

	return true;
	/*****************************/
	/* 此处写设置是否有声音的代码*/
	/*****************************/
}

bool MenuState::soundIf(const CEGUI::EventArgs &e)
{
	bool soundCheck = ((CEGUI::ToggleButton*)(static_cast<const CEGUI::WindowEventArgs&>(e).window))->isSelected();

	if(soundCheck)
	{		
		SoundMgr->play_sound = true;
		SoundMgr->play("mainMenu",0);		
		//openSoundCheckBox->setText("Ok");
	}
	else
	{		
		SoundMgr->play_sound = false;
		SoundMgr->stop("mainMenu");
		//openSoundCheckBox->setText("");

	}
	return true;

	/*****************************/
	/* 此处写设置是否有声音的代码*/
	/*****************************/
}

bool MenuState::start(const CEGUI::EventArgs &e)
{
	startbgp->hide();



	/* Author by JH */
	/*  播放视频 */
	IGraphBuilder	*pGraph;		//滤波器链表管理器
	IMediaControl	*pMediaControl;	//媒体控制接口，如run、stop、pause
	IMediaEventEx	*pMediaEvent;	//媒体事件接口
	IVideoWindow	*pVideoWindow;	//视频窗口接口

	CoInitialize(NULL);
	if (SUCCEEDED(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder, (void **)&pGraph)))
	{
		pGraph->QueryInterface(IID_IMediaControl, (void **)&pMediaControl);
		pGraph->QueryInterface(IID_IMediaEventEx, (void **)&pMediaEvent);
		pGraph->QueryInterface(IID_IVideoWindow, (void **)&pVideoWindow);
	}


	/// 创建一个能够播放指定文件的 filter graph
	pGraph->RenderFile(L"E:\\glory.mp4",NULL);
	//MessageBoxA(0,"QueryInterface","QueryInterface",0);
		/*
	HWND inWindow = GetActiveWindow();
	if (pVideoWindow)
	{
		// 首先隐藏视频窗口
		pVideoWindow->put_Visible(OAFALSE);
		pVideoWindow->put_Owner((OAHWND)inWindow);

		//获取输入窗口的显示区域
		RECT windowRect;
		::GetClientRect(inWindow, &windowRect);
		pVideoWindow->put_Left(0);
		pVideoWindow->put_Top(0);
		pVideoWindow->put_Width(windowRect.right - windowRect.left);
		pVideoWindow->put_Height(windowRect.bottom - windowRect.top);
		pVideoWindow->put_WindowStyle(WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);
		pVideoWindow->put_MessageDrain((OAHWND) inWindow);
		// 回复视频窗口
		if (inWindow != NULL)
		{
			pVideoWindow->put_Visible(OATRUE);
		}
		else
		{
			pVideoWindow->put_Visible(OAFALSE);
		}
	}

	pMediaControl->Run();
	long evCode = 0;
	pMediaEvent->WaitForCompletion(INFINITE,&evCode);
	pVideoWindow->put_Visible(OAFALSE);
	*/
	/*  播放视频 */
	/* Author by JH */
	


	changeAppState(findByName("GameState1"));
	return true;
}

bool MenuState::exitGame(const CEGUI::EventArgs &e)
{
	m_bQuit = true;
	return true;
}

bool MenuState::openSetting(const CEGUI::EventArgs &e)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(setbgp);
	return true;
}


bool MenuState::setComplete(const CEGUI::EventArgs &e)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(startbgp);
	return true;
}

void MenuState::exit()
{
	SoundMgr->stop("mainMenu");
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving MenuState...");

	m_pSceneMgr->destroyCamera(m_pCamera);
	if(m_pSceneMgr)
		OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
}


bool MenuState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
	{
		m_bQuit = true;
		return true;
	}

	OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);

	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	context.injectKeyDown((CEGUI::Key::Scan)keyEventRef.key);
	context.injectChar((CEGUI::Key::Scan)keyEventRef.text);

	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool MenuState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)keyEventRef.key);
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool MenuState::mouseMoved(const OIS::MouseEvent &evt)
{
	//if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;

	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.getDefaultGUIContext().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);
	// Scroll wheel.
	if (evt.state.Z.rel)
		sys.getDefaultGUIContext().injectMouseWheelChange(evt.state.Z.rel / 120.0f);
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool MenuState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	//if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;

	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));


	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool MenuState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
	return true;
}

void MenuState::update(double timeSinceLastFrame)
{
	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	//OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);

	if(m_bQuit == true)
	{
		shutdown();
		return;
	}
}

void MenuState::buttonHit(OgreBites::Button *button)
{
	if(button->getName() == "ExitBtn")
		m_bQuit = true;
	else if(button->getName() == "EnterBtn")
		changeAppState(findByName("GameState2"));
}

