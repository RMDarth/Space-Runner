#pragma once
#include "Basic.h"
#include "SpaceDefs.h"
#include "SpaceObject.h"
#include <string>

class SceneSector;
class ModelDrawable;

namespace CoreEngine
{
    class BossInterface : public SpaceObject
    {
    public:
        enum class Type
        {
            Small,
            Big
        };

        BossInterface(Vector3 offset)
            : SpaceObject(offset, 0)
        {
        }

        virtual void Hit() = 0;
        virtual int GetLives() = 0;
        virtual Type GetType() = 0;
    };
}