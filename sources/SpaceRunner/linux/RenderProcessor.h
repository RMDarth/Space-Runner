#pragma once
#include "PrecompileHeaders.h"
#include "OgreOverlaySystem.h"
#include "IDrawable.h"
#include "Camera.h"
#include "Light.h"
#include "LinuxDefs.h"
#define SKYBOX_NUM 5

class RenderProcessor
{
private:
	Ogre::SceneManager* _sceneManager;
	Ogre::Viewport* _viewport;
	Ogre::OverlaySystem* _overlaySystem;
	std::vector<IDrawable*> _drawableList;
	Camera* _camera;
	std::vector<Light*> _lightList;

	static RenderProcessor* _instance;

	RenderProcessor();

public:
	static RenderProcessor* Instance();

	void InitLoadingScreen();
	void RegisterDrawable(IDrawable* drawable);
	void Update(float time);
	void SetSkybox(int id);
	Camera* GetCamera();


	Light* GetLight(int id);
	int CreateLight();
	int GetLightCount();

	// OGRE SPECIFIC //
	Ogre::SceneManager* GetSceneManager();
	Ogre::Viewport* GetViewport();

private:
	void InitResources();
};

