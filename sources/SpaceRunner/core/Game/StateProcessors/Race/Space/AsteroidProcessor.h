#pragma once
#include "Basic.h"
#include "SpaceDefs.h"
#include "Game/StateProcessors/Race/LevelStructure.h"
#include <string>
#include <deque>


namespace CoreEngine
{
    class Asteroid;

    class AsteroidProcessor
    {
        float &_lastObstacleCreated;
        PtrList<Asteroid>& _asteroidList;
        LevelDifficulty _difficulty;

        std::deque<int8_t> _lastRows;

    public:
        AsteroidProcessor(PtrList<Asteroid>& asteroidList, float &lastObstacleCreated, LevelDifficulty difficulty);

        void Process(float totalTime);

    private:
        int CountCentralDoubleBlocks();
        bool isNormalMovePossible(int8_t poses);

    };
}