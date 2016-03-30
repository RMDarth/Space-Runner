#include "OgreApplication.h"

OgreApplication* OgreApplication::_instance = nullptr;

OgreApplication::OgreApplication()
{
	_root = NULL;
    _renderWindow = NULL;

	_exit = false;
}

OgreApplication* OgreApplication::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new OgreApplication();
	}
	return _instance;
}

void OgreApplication::Init(Ogre::Root *root, Ogre::RenderWindow * renderWindow)
{
	_root = root;
	_renderWindow = renderWindow;
}

void OgreApplication::WindowResized()
{
	_renderWindow->windowMovedOrResized();
}

void OgreApplication::LoopStep(float deltaTime)
{
	//Ogre::WindowEventUtilities::messagePump();

	if (_renderWindow->isClosed())
	{
		_exit = true;
		return;
	}

	if (_renderWindow->isActive())
	{
		_exit = !_root->renderOneFrame(deltaTime);
	}
}

bool OgreApplication::IsExit()
{
	return _exit;
}

Ogre::Root* OgreApplication::GetRoot()
{
	return _root;
}

Ogre::RenderWindow* OgreApplication::GetWindow()
{
	return _renderWindow;
}

