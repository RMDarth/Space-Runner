#include "RenderProcessor.h"
#include "OgreApplication.h"
#include "OgreRectangle2D.h"
#include "OgreFontManager.h"

RenderProcessor* RenderProcessor::_instance = nullptr;

RenderProcessor* RenderProcessor::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new RenderProcessor();
	}
	return _instance;
}

RenderProcessor::RenderProcessor()
{
	Ogre::Root* root = OgreApplication::Instance()->GetRoot();
	Ogre::RenderWindow * window = OgreApplication::Instance()->GetWindow();
	
	_sceneManager = root->createSceneManager(Ogre::ST_GENERIC);

	auto camera = _sceneManager->createCamera("Camera");
	_camera = new Camera(camera);
	camera->setPosition(Ogre::Vector3(0,3*3,20));
	camera->lookAt(Ogre::Vector3(0,0,0));
	camera->setNearClipDistance(5);

	_viewport = window->addViewport(camera);
	//_viewport->setBackgroundColour(Ogre::ColourValue(0.4f,0.5f,0.4f));
	camera->setAspectRatio(Ogre::Real(_viewport->getActualWidth()) / Ogre::Real(_viewport->getActualHeight()));

	_overlaySystem = new Ogre::OverlaySystem();
	_sceneManager->addRenderQueueListener(_overlaySystem);

	InitResources();
	CreateLight();
	
	_sceneManager->setSkyBox(true, "Skybox");

}

void RenderProcessor::SetSkybox(int id)
{
	char name[100];
	sprintf(name, "Skybox_%d", id);
	_sceneManager->setSkyBox(true, name);

}

void RenderProcessor::InitLoadingScreen()
{
	// Create background material
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("Background", "Loading");
	material->getTechnique(0)->getPass(0)->createTextureUnitState("loadlogo.jpg");
	material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	material->getTechnique(0)->getPass(0)->setLightingEnabled(false);

	// Create background rectangle covering the whole screen
	Ogre::Rectangle2D* rect = new Ogre::Rectangle2D(true);
	rect->setCorners(-1.0, 1.0, 1.0, -1.0);
	rect->setMaterial("Background");

	// Render the background before everything else
	rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);

	// Use infinite AAB to always stay visible
	Ogre::AxisAlignedBox aabInf;
	aabInf.setInfinite();
	rect->setBoundingBox(aabInf);

	// Attach background to the scene
	Ogre::SceneNode* node = _sceneManager->getRootSceneNode()->createChildSceneNode("Background");
	//node->attachObject(rect);
}


void RenderProcessor::InitResources()
{
	Ogre::ConfigFile cf;
    cf.load("resources.cfg");
	
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	Ogre::String sec, type, arch;
	while (seci.hasMoreElements())
	{
		sec = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap* settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;

		for (i = settings->begin(); i != settings->end(); i++)
		{
			type = i->first;
			arch = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch, type, sec);
		}
	}
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Loading");
	InitLoadingScreen();

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	auto font = Ogre::FontManager::getSingleton().getByName("GUIfont");
	font->load();
}

void RenderProcessor::RegisterDrawable(IDrawable* drawable)
{
	_drawableList.push_back(drawable);
}

void RenderProcessor::Update(float time)
{
	for_each(_drawableList.begin(), _drawableList.end(), std::bind(&IDrawable::Update, std::placeholders::_1, time));
}

Camera* RenderProcessor::GetCamera()
{
	return _camera;
}

Light* RenderProcessor::GetLight(int id)
{
	return _lightList[id];
}

int RenderProcessor::CreateLight()
{
	Ogre::Light* light = _sceneManager->createLight();
	light->setType(Ogre::Light::LT_POINT);
	light->setPosition(0,3*3,20);

	_sceneManager->setAmbientLight(Ogre::ColourValue(0.0f, 0.0f, 0.0f));
	light->setDiffuseColour(Ogre::ColourValue(1.0f,1.0f,1.0f));
	light->setSpecularColour(Ogre::ColourValue(0.5f,0.5f,0.5f));
	
	Light* lightWrapper = new Light(light);
	_lightList.push_back(lightWrapper);

	return _lightList.size() - 1;
}


int RenderProcessor::GetLightCount()
{
	return _lightList.size();
}

Ogre::SceneManager* RenderProcessor::GetSceneManager()
{
	return _sceneManager;
}

Ogre::Viewport* RenderProcessor::GetViewport()
{
	return _viewport;
}

