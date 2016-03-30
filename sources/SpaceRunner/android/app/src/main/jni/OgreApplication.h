#pragma once
#include "PrecompileHeaders.h"

class OgreApplication
{
	static OgreApplication* _instance;
	Ogre::Root *_root;
	Ogre::RenderWindow *_renderWindow;

	bool _exit;

	OgreApplication();
public:
	static OgreApplication* Instance();
	
	void Init(Ogre::Root *root, Ogre::RenderWindow * renderWindow);
	void LoopStep(float deltaTime);
	void WindowResized();

	bool IsExit();

	Ogre::Root* GetRoot();
	Ogre::RenderWindow* GetWindow();
};

