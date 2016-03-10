#include "OgreApplication.h"
#include "OgreOverlaySystem.h"

OgreApplication* OgreApplication::_instance = nullptr;

OgreApplication::OgreApplication()
{
	_root = new Ogre::Root("plugins.cfg");
	if (!_root->showConfigDialog())
	{
		_exit = true;
		return;
	}

	_renderWindow = _root->initialise(true, "Space Runner");

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


void OgreApplication::Init()
{
	
}

void OgreApplication::LoopStep(float deltaTime)
{
	Ogre::WindowEventUtilities::messagePump();

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

