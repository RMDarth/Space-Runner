#pragma once
#include "PrecompileHeaders.h"

class ModelAnimation
{
	Ogre::AnimationState * _state;
	Ogre::AnimationState * _lodState;
public:
	ModelAnimation(Ogre::AnimationState * state, Ogre::AnimationState * lodState = nullptr);
	~ModelAnimation();

	Ogre::AnimationState * GetState();
	Ogre::AnimationState * GetLODState();
	void Update(float time);

	bool hasEnded();
};

