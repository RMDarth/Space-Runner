#include "ModelAnimation.h"


ModelAnimation::ModelAnimation(Ogre::AnimationState * state, Ogre::AnimationState * lodState)
{
	_state = state;
	_lodState = lodState;
}


ModelAnimation::~ModelAnimation(void)
{
}

Ogre::AnimationState * ModelAnimation::GetState()
{
	return _state;
}

Ogre::AnimationState * ModelAnimation::GetLODState()
{
	return _lodState;
}


void ModelAnimation::Update(float time)
{
	_state->addTime(time);
	if (_lodState != nullptr)
		_lodState->addTime(time);
}

bool ModelAnimation::hasEnded()
{
	return _state->hasEnded();
}