#pragma once
#include "IDrawable.h"
#include "PrecompileHeaders.h"
#include "RenderProcessor.h"
#include "Render/SceneSector.h"
#include <vector>

class ParticleDrawable : public IDrawable
{
	Ogre::ManualObject * _manualObject;
	SceneSector * _sceneSector;

public:
	ParticleDrawable(SceneSector * sector, std::string material, float size);
	virtual ~ParticleDrawable();

	void SetMaterial(std::string name);

	virtual void Update(float deltaTime);
	virtual void SetVisible(bool value);
};