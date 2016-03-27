#pragma  once
#include <vector>
#include <memory>
#include <functional>
#include "SpaceDefs.h"
#include "Basic.h"
#define VISIBLE_NUM 15

namespace CoreEngine
{
	class SpaceLine;
	class Asteroid;
	class EnemyFighter;
	class SpaceDust;
	class Fence;
	class BlasterBurst;
	class Explosion;
	class Sparks;

	struct Level;
	struct Obstacle;

	enum class SpaceObjectType
	{
		Asteroid,
		EnemyFighter
	};

	using EventCallback = std::function<void(SpaceObjectType)>;

	class Space
	{
		PtrList<Asteroid> _backgroundAsteroidList;
		PtrList<Asteroid> _asteroidList;
		PtrList<EnemyFighter> _fighterList;

		PtrList<BlasterBurst> _shotList;
		PtrList<Explosion> _explosionList;
		PtrList<Sparks> _sparksList;
		
		UPtr<SpaceDust> _spaceDust;
		UPtr<Fence> _fence;
		UPtr<Level> _currentLevel;
		UPtr<Obstacle> _currentObstacle;

		EventCallback _shotCallback;
		
		float _totalTime;
		float _lastObstacleCreated;
	public:
		Space();
		void GenerateSpace();
		void Update(float time, float speed);
		void SetVisible(bool visible);
		bool IsIntersected(float turn);
		void AddShot(Vector3 pos, float speed);
		
		void RegisterShotEvent(EventCallback callback);
	private:
		void AddObstacles(float time);
		
		void AddAsteroids(float time);
		void AddEnemyFighter(float time);

		void UpdateShots(float time, float roadOffset);
	};
}