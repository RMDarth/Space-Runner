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

        enum class Difficulty
        {
            Easy,
            Normal,
            Hard
        };

        BossInterface(Vector3 offset, Difficulty difficulty)
            : SpaceObject(offset, 0)
            , _difficulty(difficulty)
        {
        }

        virtual void Hit() = 0;
        virtual int GetLives() = 0;
        virtual Type GetType() = 0;

        virtual Difficulty GetDifficulty() { return _difficulty; }

    protected:
        Difficulty _difficulty;
    };
}