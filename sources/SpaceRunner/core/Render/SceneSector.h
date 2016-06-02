#pragma once
#include "RenderProcessor.h"

class SceneSector
{
	Ogre::SceneNode * _node;
public:
	SceneSector(Ogre::SceneNode* node);
	~SceneSector();

	void SetScale(Vector3 scale);
	Ogre::SceneNode* GetNode();
};

