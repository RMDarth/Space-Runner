#include "Space.h"
#include "SpaceDust.h"
#include "Fence.h"
#include "Asteroid.h"
#include "EnemyFighter.h"
#include "SceneSector.h"

#include "LevelStructure.h"

#include <memory>
#include <algorithm>
#include <functional>
using namespace std;

namespace CoreEngine
{
	static const float presetPos[3] = { -BLOCK_SIZE * 1.1f, 0, BLOCK_SIZE * 1.1f };

	Space::Space()
	{
		_lastObstacleCreated = 0;
		_spaceDust = make_unique<SpaceDust>("SpaceDust");
		_fence = make_unique<Fence>();
		GenerateSpace();
	}

	void Space::GenerateSpace()
	{
		for (int i = 0; i < ASTEROID_NUM; i++)
		{
			float posX = 0;
			while (posX > -BLOCK_SIZE*1.5f && posX < BLOCK_SIZE*1.5f)
			{
				posX = (rand() % (int)(BLOCK_SIZE * 10.0f * 10)) / 10.0f - BLOCK_SIZE * 5.0f;
			}
			auto posY = (rand() % (int)(BLOCK_SIZE * 10.0f * 10)) / 10.0f - BLOCK_SIZE * 5.0f;
			auto asteroid = make_shared<Asteroid>(Vector3(-i*BLOCK_SIZE, posY, posX), "moon.mesh", 0.0f);
			_backgroundAsteroidList.push_back(asteroid);
		}

		vector<ObstacleType> obstacleList;
		for (int i = 0; i < 50; i++)
		{
			int type = rand() % 10;
			if (type < 2)
			{
				obstacleList.push_back(ObstacleType::EnemyFighter);
			}
			else
			{
				obstacleList.push_back(ObstacleType::AsteroidsPack);
			}
		}
		_currentLevel = make_unique<Level>();
		_currentLevel->currentObstacle = 0;
		_currentLevel->obstacleList = obstacleList;
		_currentLevel->skyboxId = 0;
		_currentLevel->energyToComplete = 100;
	}

	void Space::Update(float time, float speed)
	{
		_totalTime += time * speed;
		float roadOffset = time * speed * 5.0f;

		_spaceDust->Update(time, roadOffset);
		_fence->Update(time, roadOffset);
		
		AddObstacles(_totalTime);
		
		for_each(_backgroundAsteroidList.begin(), _backgroundAsteroidList.end(), bind(&Asteroid::TryReset, placeholders::_1));
		for_each(_backgroundAsteroidList.begin(), _backgroundAsteroidList.end(), bind(&Asteroid::Update, placeholders::_1, time, roadOffset));
		
		for_each(_asteroidList.begin(), _asteroidList.end(), bind(&Asteroid::Update, placeholders::_1, time, roadOffset));
		_asteroidList.erase(remove_if(_asteroidList.begin(), _asteroidList.end(), bind(&Asteroid::IsDone, placeholders::_1)), _asteroidList.end());

		for_each(_fighterList.begin(), _fighterList.end(), bind(&EnemyFighter::Update, placeholders::_1, time, roadOffset));
		_fighterList.erase(remove_if(_fighterList.begin(), _fighterList.end(), bind(&EnemyFighter::IsDone, placeholders::_1)), _fighterList.end());
	}

	void Space::AddObstacles(float totalTime)
	{
		if (!_currentObstacle || (totalTime - _currentObstacle->timeStarted) > _currentObstacle->timeLength )
		{
			// create new obstacle
			_currentLevel->currentObstacle++;
			if (_currentLevel->currentObstacle == _currentLevel->obstacleList.size())
				_currentLevel->currentObstacle = 0;

			switch (_currentLevel->obstacleList[_currentLevel->currentObstacle])
			{
			case ObstacleType::AsteroidsPack:
				{
					_currentObstacle = make_unique<Obstacle>();
					int count = rand() % 5 + 2;
					_lastObstacleCreated = totalTime;

					_currentObstacle->type = ObstacleType::AsteroidsPack;
					_currentObstacle->timeLength = count * 4 + 1;
					_currentObstacle->timeStarted = totalTime;
					_currentObstacle->energyCount = count;
				}
				break;

			case ObstacleType::EnemyFighter:
				_currentObstacle = make_unique<Obstacle>();
				_currentObstacle->type = ObstacleType::EnemyFighter;
				_currentObstacle->timeLength = 25;
				_currentObstacle->timeStarted = totalTime;
				_currentObstacle->energyCount = 0;
				break;
			}
		}
		else 
		{
			switch (_currentObstacle->type)
			{
			case ObstacleType::AsteroidsPack:
				AddAsteroids(totalTime);
				break;

			case ObstacleType::EnemyFighter:
				AddEnemyFighter(totalTime);
				break;
			}
		}
	}

	void Space::AddAsteroids(float totalTime)
	{
		if (totalTime > 3 && totalTime - _lastObstacleCreated > 4)
		{
			bool posUsed[3] = { false, false, false };
			
			int count = rand() % 2 + 1;
			for (int i = 0; i < count; i++)
			{
				int posIndex;
				do{
					posIndex = rand() % 3;
				} while (posUsed[posIndex]);

				float deviation = (rand() % 10 - 5.0f) / 10.0f;
				float pos = presetPos[posIndex] + deviation;

				int anum = rand() % 6 + 1;
				stringstream ss;
				ss << "Asteroid" << anum << "_LOD0.mesh";

				Asteroid * asteroid = new Asteroid(Vector3(-ASTEROID_NUM * BLOCK_SIZE, 0, pos), ss.str(), 0.0f, 5.0f);
				_asteroidList.push_back(shared_ptr<Asteroid>(asteroid));
			}
			_lastObstacleCreated = totalTime;
		}
	}

	void Space::AddEnemyFighter(float totalTime)
	{
		if (totalTime > 3 && totalTime - _lastObstacleCreated > 10)
		{
			int posIndex = rand() % 3;
			float pos = presetPos[posIndex];

			auto fighter = make_shared<EnemyFighter>(Vector3(-ASTEROID_NUM * BLOCK_SIZE, 0, pos), "ship.mesh", "ShipMaterialYellow", 0.2f, 8.0f);
			_fighterList.push_back(fighter);

			_lastObstacleCreated = totalTime + 10;
		}
	}

	void Space::SetVisible(bool visible)
	{
		for_each(_asteroidList.begin(), _asteroidList.end(), bind(&Asteroid::SetVisible, placeholders::_1, visible));
		for_each(_fighterList.begin(), _fighterList.end(), bind(&EnemyFighter::SetVisible, placeholders::_1, visible));
		for_each(_backgroundAsteroidList.begin(), _backgroundAsteroidList.end(), bind(&Asteroid::SetVisible, placeholders::_1, visible));
		_spaceDust->SetVisible(visible);
	}

	bool Space::IsIntersected(float turn)
	{
		for (auto i = _asteroidList.begin(); i != _asteroidList.end(); i++)
		{
			if ((*i)->IsIntersected(turn))
			{
				(*i)->Update(0, 15.0f);
				return true;
			}
		}

		for (auto i = _fighterList.begin(); i != _fighterList.end(); i++)
		{
			if ((*i)->IsIntersected(turn))
			{
				(*i)->Update(0, 15.0f);
				return true;
			}
		}
		return false;
	}

}