#pragma once
#include "IDrawable.h"
#include "PrecompileHeaders.h"
#include "RenderProcessor.h"
#include "Render/SceneSector.h"
#include <vector>

class RectDrawable : public IDrawable
{
	Ogre::ManualObject * _manualObject;
	SceneSector * _sceneSector;

public:
	RectDrawable(SceneSector * sector, std::string material, std::vector<Vector3> pointlist);
	virtual ~RectDrawable();

	void SetMaterial(std::string name);

	virtual void Update(float deltaTime);
	virtual void SetVisible(bool value);
};