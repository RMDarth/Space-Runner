#pragma  once
#include <vector>
#include <memory>
#include <functional>
#include "core/Game/StateProcessors/Race/LevelStructure.h"
#include "SpaceDefs.h"
#include "Basic.h"
#define VISIBLE_NUM 15

namespace CoreEngine
{
    class SpaceLine;
    class Asteroid;
    class AsteroidProcessor;
    class Mine;
    class Cruiser;
    class SpaceDust;
    class Fence;
    class BlasterBurst;
    class Explosion;
    class Sparks;
    class EnergyOrb;
    class Barrier;
    class Missile;
    class BossInterface;

    struct Level;
    struct Obstacle;

    using EventCallback = std::function<void(SpaceObjectType)>;
    using BossCallback = std::function<void(int,int)>; // curlives, maxlives

    class Space
    {
        PtrList<Asteroid> _backgroundAsteroidList;
        PtrList<Asteroid> _asteroidList;
        PtrList<Mine> _mineList;
        PtrList<Cruiser> _cruiserList;
        PtrList<EnergyOrb> _orbList;
        PtrList<Barrier> _barrierList;
        Ptr<BossInterface> _boss;

        PtrList<BlasterBurst> _shotList;
        PtrList<Missile> _missileList;
        PtrList<Explosion> _explosionList;
        PtrList<Sparks> _sparksList;

        Ptr<AsteroidProcessor> _asteroidProcessor;

        UPtr<SpaceDust> _spaceDust;
        UPtr<Fence> _fence;
        UPtr<Level> _currentLevel;

        UPtr<Obstacle> _currentObstacle;
        std::shared_ptr<Prefab> _currentPrefab;

        EventCallback _shotCallback;
        BossCallback _bossCallback;

        float _totalTime;
        float _lastObstacleCreated;
        int _lastObstaclePos;
        float _lastPosChange;
    public:
        Space();
        void GenerateSpace();
        void Update(float time, float speed);
        void SetVisible(bool visible);
        SpaceObjectType IsIntersected(float turn);
        void AddShot(Vector3 pos, float speed);

        void PreloadModels();

        void RegisterShotEvent(EventCallback callback);
        void RegisterBossEvent(BossCallback callback);
        Level* GetCurrentLevel();
    private:
        void AddObstacles(float time);
        void GenerateLevel(LevelDifficulty difficulty);

        void ProcessAsteroids(float time);
        void ProcessMine(float time);
        void ProcessEnemyCruiser(float time);
        void ProcessEnergyOrbs(float time);
        void ProcessPrefabs(float time);
        void ProcessPrefabEnergyOrbs(float time);
        void ProcessEnergyBarrier(float time);
        void ProcessBoss(float time);
        void ProcessBigBoss(float time);

        void UpdateShots(float time, float roadOffset);
    };
}