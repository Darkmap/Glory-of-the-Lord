//|||||||||||||||||||||||||||||||||||||||||||||||

#include "DemoApp.hpp"
#include "PxPhysicsAPI.h"
#include "SoundManager.h"
#include "Physx.h"

#include "AppState.hpp"

#include "ParticleSystem.hpp"
#include <iostream>
using namespace physx;
using namespace std;

#pragma comment(lib,"PhysX3Common_x86.lib")
#pragma comment(lib, "PhysX3_x86.lib")
#pragma comment(lib,"PhysX3Cooking_x86.lib")
#pragma comment(lib,"PhysX3PROFILE_x86.lib")
#pragma comment(lib,"PhysX3CharacterKinematic_x86.lib")
#pragma comment(lib,"PxTask.lib")
#pragma comment(lib,"PhysXProfileSDK.lib")
#pragma comment(lib,"PhysX3Extensions.lib")

/*  Author by JH */
Physx * mphysx;
Ogre::Timer waveTimer;
SoundManager * SoundMgr;
/*  Author by JH */


CEGUI::Renderer * AppState::mRenderer = NULL;

int ParticleSystemTools::shotPS = 0;
int ParticleSystemTools::snowPS = 0;
int ParticleSystemTools::ringPS = 0;
int ParticleSystemTools::bangPS = 0;


//|||||||||||||||||||||||||||||||||||||||||||||||

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char **argv)
#endif
{
	DemoApp demo;
	try	
	{

		/*  Author by JH */
		mphysx = new Physx();
		SoundMgr = new SoundManager();

		
		/*  Author by JH */
		/*  Author by GL */
		SoundMgr->open("./count.wav","count");
		
		SoundMgr->open("./fight(enemy).wav","fight");
		SoundMgr->open("./mainMenu.wav","mainMenu");
		//SoundMgr->open("./run.wav","run");
		SoundMgr->open("./arrow.wav","arrow");
		//SoundMgr->open("./fight.wav","fight");

		/*  Author by GL */

		demo.startDemo();
	}
	catch(std::exception& e)
	{
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBoxA(NULL, e.what(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		fprintf(stderr, "An exception has occurred: %s\n", e.what());
#endif
	}
	
	return 0;
}

//|||||||||||||||||||||||||||||||||||||||||||||||