#pragma once
#include "PrecompileHeaders.h"
#include "Basic.h"
using namespace CoreEngine;

class Light
{
	Ogre::Light * _light;
public:
	Light(Ogre::Light * light);
	~Light();

	void SetPosition(Vector3 pos);
	Vector3 GetPosition();
};

