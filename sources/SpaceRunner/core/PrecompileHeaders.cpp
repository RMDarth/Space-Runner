#include "PrecompileHeaders.h"

Ogre::Vector3 VectorToOgre(CoreEngine::Vector3 vector)
{
	return Ogre::Vector3(vector.x, vector.y, vector.z);
}

CoreEngine::Vector3 VectorFromOgre(Ogre::Vector3 vector)
{
	return CoreEngine::Vector3(vector.x, vector.y, vector.z);
}