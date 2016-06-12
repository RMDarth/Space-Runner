#include "RenderProcessor.h"
#include "OgreApplication.h"
#include "RTShaderHelper.h"
#include "OgreFontManager.h"
#include "android_native_app_glue.h"
#include <stdio.h>

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
	_viewport = NULL;
	_camera = NULL;
	_sceneManager = NULL;
}



void RenderProcessor::Init(int dpi)
{
	Ogre::Root* root = OgreApplication::Instance()->GetRoot();
	Ogre::RenderWindow * window = OgreApplication::Instance()->GetWindow();
	_dpi = dpi;
	
	_sceneManager = root->createSceneManager(Ogre::ST_GENERIC);  

	auto camera = _sceneManager->createCamera("Camera");
	_camera = new Camera(camera);
	camera->setPosition(Ogre::Vector3(0,3*3,20));
	camera->lookAt(Ogre::Vector3(0,0,0));
	camera->setNearClipDistance(5);

	_viewport = window->addViewport(camera);
	_viewport->setBackgroundColour(Ogre::ColourValue(0.0f,0.0f,0.0f));
	_viewport->setMaterialScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
	camera->setAutoAspectRatio(true);

	_overlaySystem = new Ogre::OverlaySystem();
	_sceneManager->addRenderQueueListener(_overlaySystem);
}

void RenderProcessor::PostInit()
{
	CreateLight();

	_sceneManager->setSkyBox(true, "Skybox");

	auto font = Ogre::FontManager::getSingleton().getByName("GUIfont");
	//font->setTrueTypeResolution(_dpi);
	font->load();
}

void RenderProcessor::SetSkybox(int id)
{
	char name[100];
	sprintf(name, "Skybox_%d", id);
	_sceneManager->setSkyBox(true, name);

}

void RenderProcessor::InitResources()
{
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Ball", "FileSystem");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Skybox", "FileSystem");
	//Ogre::ResourceGroupManager::getSingleton().addResourceLocation("ball.zip", "Zip");
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

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

