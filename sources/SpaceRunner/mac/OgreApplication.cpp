#include "OgreApplication.h"
#include "OgreOverlaySystem.h"

#include "OgreParticleFXPlugin.h"
#include "OgreGLPlugin.h"
#include "OgreCgPlugin.h"

OgreApplication* OgreApplication::_instance = nullptr;

OgreApplication::OgreApplication()
{
    //_root = new Ogre::Root();
	_root = new Ogre::Root();

    _staticLoader = new Ogre::StaticPluginLoader();
    _staticLoader->load();

    auto mGLPlugin = new Ogre::GLPlugin();
    _root->installPlugin(mGLPlugin);

	auto mParticleFXPlugin = new Ogre::ParticleFXPlugin();
	_root->installPlugin(mParticleFXPlugin);

	auto mCgPlugin = new Ogre::CgPlugin();
	_root->installPlugin(mCgPlugin);

    _root->setRenderSystem(_root->getAvailableRenderers().at(0));
    //cout << (_root->getAvailableRenderers().at(0)->getName().c_str());

	if (!_root->showConfigDialog())
	{
		_exit = true;
		return;
	}

	_renderWindow = _root->initialise(true, "Space Rush 3D");

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

	_exit = !_root->renderOneFrame(deltaTime);
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

