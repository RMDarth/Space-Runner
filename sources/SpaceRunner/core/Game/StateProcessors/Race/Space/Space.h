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
	class EnemyFighter;
	class Cruiser;
	class SpaceDust;
	class Fence;
	class BlasterBurst;
	class Explosion;
	class Sparks;
	class EnergyOrb;
	class Barrier;
	class Missile;

	struct Level;
	struct Obstacle;

	using EventCallback = std::function<void(SpaceObjectType)>;

	class Space
	{
		PtrList<Asteroid> _backgroundAsteroidList;
		PtrList<Asteroid> _asteroidList;
		PtrList<EnemyFighter> _fighterList;
		PtrList<Cruiser> _cruiserList;
		PtrList<EnergyOrb> _orbList;
		PtrList<Barrier> _barrierList;

		PtrList<BlasterBurst> _shotList;
		PtrList<Missile> _missileList;
		PtrList<Explosion> _explosionList;
		PtrList<Sparks> _sparksList;
		
		UPtr<SpaceDust> _spaceDust;
		UPtr<Fence> _fence;
		UPtr<Level> _currentLevel;

		UPtr<Obstacle> _currentObstacle;
		std::shared_ptr<Prefab> _currentPrefab;

		EventCallback _shotCallback;
		
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
		Level* GetCurrentLevel();
	private:
		void AddObstacles(float time);
		
		void AddAsteroids(float time);
		void AddEnemyFighter(float time);
		void AddEnemyCruiser(float time);
		void AddEnergyOrbs(float time);
		void AddPrefabEnergyOrbs(float time);
		void AddEnergyBarrier(float time);
		void AddPrefab(float time);

		void UpdateShots(float time, float roadOffset);
	};
}