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

	class Space
	{
		PtrList<Asteroid> _backgroundAsteroidList;
		PtrList<Asteroid> _asteroidList;
		std::unique_ptr<SpaceDust> _spaceDust;
		
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