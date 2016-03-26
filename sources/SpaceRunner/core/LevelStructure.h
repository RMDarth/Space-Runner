#include <vector>
#pragma once

namespace CoreEngine
{
	enum class ObstacleType
	{
		AsteroidsPack = 0,
		EnemyFighter,
		EnergyPack,
		EnergyLine,
		Last
	};

	struct Obstacle
	{
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