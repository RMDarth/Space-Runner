#pragma once
#include "IDrawable.h"
#include "PrecompileHeaders.h"
#include "RenderProcessor.h"
#include "SceneSector.h"
#include <vector>

class LineDrawable : public IDrawable
{
	Ogre::ManualObject * _manualObject;
	SceneSector * _sceneSector;

	static Ogre::MaterialPtr _material;

public:
	LineDrawable(SceneSector * sector, std::vector<Vector3> posList);
	virtual ~LineDrawable();

	void SetMaterial(std::string name);

	virtual void Update(float deltaTime);
	virtual void SetVisible(bool value);
};

