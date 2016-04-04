#include <vector>
#pragma once

namespace CoreEngine
{
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