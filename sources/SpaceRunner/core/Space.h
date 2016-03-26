#pragma  once
#include <vector>
#include <memory>
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

	struct Level;
	struct Obstacle;

	class Space
	{
		PtrList<Asteroid> _backgroundAsteroidList;
		PtrList<Asteroid> _asteroidList;
		PtrList<EnemyFighter> _fighterList;

		PtrList<BlasterBurst> _shotList;
		//PtrList<Explosion> _explosionList;
		
		UPtr<SpaceDust> _spaceDust;
		UPtr<Fence> _fence;
		UPtr<Level> _currentLevel;
		UPtr<Obstacle> _currentObstacle;
		
		float _totalTime;
		float _lastObstacleCreated;
	public:
		Space();
		void GenerateSpace();
		void Update(float time, float speed);
		void SetVisible(bool visible);
		bool IsIntersected(float turn);
		void AddShot(Vector3 pos, float speed);
	private:
		void AddObstacles(float time);
		
		void AddAsteroids(float time);
		void AddEnemyFighter(float time);

		void UpdateShots(float time, float roadOffset);
	};
}