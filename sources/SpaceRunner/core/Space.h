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

	class Space
	{
		PtrList<Asteroid> _backgroundAsteroidList;
		PtrList<Asteroid> _asteroidList;
		PtrList<EnemyFighter> _fighterList;
		
		std::unique_ptr<SpaceDust> _spaceDust;
		std::unique_ptr<Fence> _fence;
		
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
	};
}