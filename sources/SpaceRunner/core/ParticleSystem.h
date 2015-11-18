#pragma once
#include "RenderProcessor.h"

class ParticleSystem
{
	Ogre::SceneNode * _node;
	Ogre::ParticleSystem *_system;
	float _time;

	static int _count;
	
public:
	ParticleSystem(Ogre::SceneNode* node, char* nameTemplate, char* particleTemplate, float time);

	void Update(float deltaTime);
	bool IsFinished();
	void FastForward(float time);
};

