#pragma once

#include <OgreStaticPluginLoader.h>
#include "PrecompileHeaders.h"

class OgreApplication
{
	static OgreApplication* _instance;
	Ogre::Root *_root;
	Ogre::RenderWindow *_renderWindow;
    Ogre::StaticPluginLoader* _staticLoader;

	bool _exit;

	OgreApplication();
public:
	static OgreApplication* Instance();

	void Init();
	void LoopStep(float deltaTime);

	bool IsExit();

	Ogre::Root* GetRoot();
	Ogre::RenderWindow* GetWindow();
};

