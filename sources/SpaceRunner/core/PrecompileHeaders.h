#pragma once

#include "Ogre.h"
#include "OIS/OIS.h"
#include "Basic.h"
#include <string>


Ogre::Vector3 VectorToOgre(CoreEngine::Vector3 vector);
CoreEngine::Vector3 VectorFromOgre(Ogre::Vector3 vector);
