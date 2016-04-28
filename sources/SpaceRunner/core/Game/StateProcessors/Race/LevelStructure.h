#pragma once
#include <vector>
#include <string>

namespace CoreEngine
{
    class Prefab;

    enum class SpaceObjectType : int8_t
    {
        Asteroid = 0,
        Mine = 1,
        EnemyCruiser = 2,
        EnergyOrb = 3,
        Barrier = 4,
        Missile = 5,
        None = 6
    };

    enum class ObstacleType
    {
        AsteroidsPack = 0,
        EnemyFighter = 1,
        EnemyCruiser = 2,
        EnergyOrb = 3,
        EnergyBarrier = 4,
        Prefab = 5,
        Boss = 6,
        BigBoss = 7,
        Last
    };

    enum class LevelDifficulty
    {
        Easy = 0,
        Normal = 1,
        Hard = 2
    };

    struct Obstacle
    {
        Obstacle(ObstacleType type, float timeLength, float timeStarted, int energyCount)
            : type(type), timeLength(timeLength), timeStarted(timeStarted), energyCount(energyCount)
        {

        }

        ObstacleType type;
        float timeLength;
        float timeStarted;
        int energyCount;
    };

    struct PrefabInfo
    {
        std::string category;
        int prefabNum;
    };

    struct Level
    {
        std::vector<ObstacleType> obstacleList;
        int currentObstacle;
        std::vector<PrefabInfo> prefabList;
        int currentPrefab;
        LevelDifficulty difficulty;
        int skyboxId;
        int energyToComplete;
        // int bossId;
    };

}