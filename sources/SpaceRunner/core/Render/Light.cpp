#include "Light.h"


Light::Light(Ogre::Light * light)
{
	_light = light;
}


Light::~Light()
{
}


void Light::SetPosition(Vector3 pos)
{
	_light->setPosition(VectorToOgre(pos));
}

Vector3 Light::GetPosition()
{
	return VectorFromOgre(_light->getPosition());
}