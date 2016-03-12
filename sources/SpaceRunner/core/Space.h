#pragma  once
#include <vector>
#include <memory>
#include "SpaceDefs.h"
#define VISIBLE_NUM 15

namespace CoreEngine
{
	class SpaceLine;
	class Asteroid;
	class SpaceDust;
	class Fence;

	class Space
	{
		PtrList<Asteroid> _backgroundAsteroidList;
		PtrList<Asteroid> _asteroidList;
		std::unique_ptr<SpaceDust> _spaceDust;
		std::unique_ptr<Fence> _fence;
		
		float _totalTime;
		float _lastAsteroidCreated;
	public:
		Space();
		void GenerateSpace();
		void Update(float time, float speed);
		void SetVisible(bool visible);
		bool IsIntersected(float turn);
	private:
		void AddAsteroids(float time);
	};
}