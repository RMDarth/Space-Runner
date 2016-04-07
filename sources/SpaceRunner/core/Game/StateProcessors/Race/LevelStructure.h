#include <vector>
#pragma once

namespace CoreEngine
{
	enum class SpaceObjectType : int8_t
	{
		Asteroid = 0,
		EnemyFighter = 1,
		EnemyCruiser = 2,
		EnergyOrb = 3,
		Barrier = 4,
		Missile = 5,
		None = 6
	};

	enum class ObstacleType
	{
		AsteroidsPack = 0,
		EnemyFighter,
		EnemyCruiser,
		EnergyOrb,
		EnergyBarrier,
		Last
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

	struct Level
	{
		std::vector<ObstacleType> obstacleList;
		int currentObstacle;
		int skyboxId;
		int energyToComplete;
		// int bossId;
	};

}