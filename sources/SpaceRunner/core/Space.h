#pragma  once
#include <vector>
#include <memory>
#include "SpaceDefs.h"
#define VISIBLE_NUM 15

namespace CoreEngine
{
	class SpaceLine;
	class Asteroid;
	class EnemyFighter;
	class SpaceDust;
	class Fence;

	struct Level;
	struct Obstacle;

	class Space
	{
		PtrList<Asteroid> _backgroundAsteroidList;
		PtrList<Asteroid> _asteroidList;
		PtrList<EnemyFighter> _fighterList;
		
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
	private:
		void AddObstacles(float time);
		
		void AddAsteroids(float time);
		void AddEnemyFighter(float time);
	};
}