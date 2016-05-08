#pragma once

#include <Render/Drawables/IDrawable.h>
#include "PrecompileHeaders.h"
#include "Render/Camera.h"
#include "Render/Light.h"
#include "OgreOverlaySystem.h"
#include "AndroidDefs.h"
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
	int _dpi;

	static RenderProcessor* _instance;

	RenderProcessor();

public:
	static RenderProcessor* Instance();
	void Init(int dpi);
	void PostInit();

	void RegisterDrawable(IDrawable* drawable);
	void Update(float time);
	Camera* GetCamera();
	void SetSkybox(int id);


	Light* GetLight(int id);
	int CreateLight();
	int GetLightCount();

	// OGRE SPECIFIC //
	Ogre::SceneManager* GetSceneManager();
	Ogre::Viewport* GetViewport();

private:
	void InitResources();
};

