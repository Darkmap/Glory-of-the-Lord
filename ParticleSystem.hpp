#ifndef _PAETICLE_SYSTEM_TOOLS_H_

#define _PAETICLE_SYSTEM_TOOLS_H_

#include <iostream>
#include <OgreParticleSystem.h>
#include <OgreSceneManager.h>

class ParticleSystemTools
{
public:
	ParticleSystemTools(){
	}

	static  Ogre::ParticleSystem* getShot(Ogre::SceneManager *m_SceneMgr){

		if(shotPS<5){
			shotPS++;
		}
		else{
			shotPS=1;
		}

		Ogre::String psName = "Shot"+Ogre::StringConverter::toString(shotPS);

		if(m_SceneMgr->hasParticleSystem(psName))
			m_SceneMgr->destroyParticleSystem(psName);

		return m_SceneMgr->createParticleSystem(psName, "Examples/shot");
	}

	static  Ogre::ParticleSystem* getWind(Ogre::SceneManager *m_SceneMgr){

		if(m_SceneMgr->hasParticleSystem("Wind"))
			m_SceneMgr->destroyParticleSystem("Wind");

		return m_SceneMgr->createParticleSystem("Wind", "Examples/Aureola2");
	}

	static  Ogre::ParticleSystem* getRedRing(Ogre::SceneManager *m_SceneMgr){

		if(ringPS<3){
			ringPS++;
		}
		else{
			ringPS=1;
		}

		Ogre::String psName = "Ring"+Ogre::StringConverter::toString(ringPS);

		if(m_SceneMgr->hasParticleSystem(psName))
			m_SceneMgr->destroyParticleSystem(psName);

		return m_SceneMgr->createParticleSystem(psName, "Examples/BossMagic");
	}

	static  Ogre::ParticleSystem* getBang(Ogre::SceneManager *m_SceneMgr){

		if(bangPS<3){
			bangPS++;
		}
		else{
			bangPS=1;
		}

		Ogre::String psName = "Bang"+Ogre::StringConverter::toString(bangPS);

		if(m_SceneMgr->hasParticleSystem(psName))
			m_SceneMgr->destroyParticleSystem(psName);

		return m_SceneMgr->createParticleSystem(psName, "Examples/Bang");
	}
protected:

private:
	static int shotPS;
	static int snowPS;
	static int ringPS;
	static int bangPS;
};


#endif