#pragma once
#include "IDrawable.h"
#include "RenderProcessor.h"
#include "SceneSector.h"
#include "ModelAnimation.h"
#include <string>

class ModelDrawable : public IDrawable
{
private:
	Ogre::Entity * _entity;
	SceneSector * _sceneSector;
	std::vector<ModelAnimation> _stateList;

public:
	ModelDrawable(SceneSector * sector, std::string name);
	virtual ~ModelDrawable();

	virtual void Update(float deltaTime);
	virtual void SetVisible(bool value);
	virtual void SetRenderingQueue(int queue);
	
	std::string GetMaterialName();
	bool HasAnimation();
	bool HasLOD();
	ModelAnimation AddAnimation(std::string name);
	void RunAnimation(ModelAnimation anim, bool cycle);
	void StopAnimation(ModelAnimation anim);
	void SetAnimationFinish();

	void SetMaterial(std::string name, int lod = 0);
	void SetScale(float scale);
	void SetScale(Vector3 scale);
	void GenerateLods();
};

