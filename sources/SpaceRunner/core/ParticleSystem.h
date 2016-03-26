#pragma once
#include "RenderProcessor.h"

class ParticleSystem
{
	Ogre::SceneNode * _node;
	Ogre::ParticleSystem *_system;
	float _time;

	static int _count;
	
public:
	ParticleSystem(Ogre::SceneNode* node, std::string nameTemplate, std::string particleTemplate, float time, int priority = 0);
	virtual ~ParticleSystem();

	void Update(float deltaTime);
	bool IsFinished();
	void FastForward(float time);
};

