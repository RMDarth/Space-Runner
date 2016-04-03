#include "Space.h"
#include "SpaceDust.h"
#include "Fence.h"
#include "Asteroid.h"
#include "EnemyFighter.h"
#include "BlasterBurst.h"
#include "Explosion.h"
#include "Sparks.h"
#include "EnergyOrb.h"
#include "Barrier.h"

#include "LevelStructure.h"

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
			while (posX > -BLOCK_SIZE*15.0f && posX < BLOCK_SIZE*15.0f)
			{
				posX = (rand() % (int)(BLOCK_SIZE * 50.0f * 10)) / 10.0f - BLOCK_SIZE * 25.0f;
			}
			auto posY = (rand() % (int)(BLOCK_SIZE * 40.0f * 10)) / 10.0f - BLOCK_SIZE * 20.0f;

            int num = 1;//rand()%6 + 1;
			auto asteroid = make_shared<Asteroid>(Vector3(-i*BLOCK_SIZE * 10, posY, posX), Asteroid::getAsteroidName(num), 0.0f, 90.0);
            std::stringstream ss;
            ss << "asteroid" << num << "_Dark";
            asteroid->SetMaterial(ss.str());
			_backgroundAsteroidList.push_back(asteroid);
		}

		vector<ObstacleType> obstacleList;
		for (int i = 0; i < 50; i++)
		{
			int type = rand() % 14;
			if (type < 2)
			{
				obstacleList.push_back(ObstacleType::EnergyBarrier);
			} else
			if (type < 4)
			{
				obstacleList.push_back(ObstacleType::EnemyFighter);
			}
			else if (type < 6)
			{
				obstacleList.push_back(ObstacleType::EnergyOrb);
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
		UpdateShots(time, roadOffset);

		for_each(_backgroundAsteroidList.begin(), _backgroundAsteroidList.end(), bind(&Asteroid::TryReset, placeholders::_1));
		for_each(_backgroundAsteroidList.begin(), _backgroundAsteroidList.end(), bind(&Asteroid::Update, placeholders::_1, time, roadOffset));
		
		for_each(_asteroidList.begin(), _asteroidList.end(), bind(&Asteroid::Update, placeholders::_1, time, roadOffset));
		_asteroidList.erase(remove_if(_asteroidList.begin(), _asteroidList.end(), bind(&Asteroid::IsDone, placeholders::_1)), _asteroidList.end());

		for_each(_fighterList.begin(), _fighterList.end(), bind(&EnemyFighter::Update, placeholders::_1, time, roadOffset));
		_fighterList.erase(remove_if(_fighterList.begin(), _fighterList.end(), bind(&EnemyFighter::IsDone, placeholders::_1)), _fighterList.end());

		for_each(_explosionList.begin(), _explosionList.end(), bind(&Explosion::Update, placeholders::_1, time, roadOffset));
		_explosionList.erase(remove_if(_explosionList.begin(), _explosionList.end(), bind(&Explosion::IsDone, placeholders::_1)), _explosionList.end());

		for_each(_sparksList.begin(), _sparksList.end(), bind(&Sparks::Update, placeholders::_1, time, roadOffset));
		_sparksList.erase(remove_if(_sparksList.begin(), _sparksList.end(), bind(&Sparks::IsDone, placeholders::_1)), _sparksList.end());

		for_each(_orbList.begin(), _orbList.end(), bind(&EnergyOrb::Update, placeholders::_1, time, roadOffset));
		_orbList.erase(remove_if(_orbList.begin(), _orbList.end(), bind(&EnergyOrb::IsDone, placeholders::_1)), _orbList.end());

		for_each(_barrierList.begin(), _barrierList.end(), bind(&Barrier::Update, placeholders::_1, time, roadOffset));
		_barrierList.erase(remove_if(_barrierList.begin(), _barrierList.end(), bind(&Barrier::IsDone, placeholders::_1)), _barrierList.end());
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
					int count = rand() % 5 + 2;
					_lastObstacleCreated = totalTime;

					_currentObstacle = make_unique<Obstacle>(
						ObstacleType::AsteroidsPack,
						count * 4 + 1,
						totalTime,
						count);
				}
				break;

			case ObstacleType::EnemyFighter:
				_currentObstacle = make_unique<Obstacle>(
					ObstacleType::EnemyFighter, 
					25, 
					totalTime,
					0);
				break;

			case ObstacleType::EnergyOrb:
				_lastObstaclePos = rand() % 3;
				_lastPosChange = totalTime;
				_currentObstacle = make_unique<Obstacle>(
					ObstacleType::EnergyOrb,
					20,
					totalTime,
					0);

				break;

			case ObstacleType::EnergyBarrier:
				_currentObstacle = make_unique<Obstacle>(
					ObstacleType::EnergyBarrier,
					15,
					totalTime,
					0);

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

			case ObstacleType::EnergyOrb:
				AddEnergyOrbs(totalTime);
				break;

			case ObstacleType::EnemyFighter:
				AddEnemyFighter(totalTime);
				break;

			case ObstacleType::EnergyBarrier:
				AddEnergyBarrier(totalTime);
				break;
			}
		}
	}

	void Space::AddShot(Vector3 pos, float speed)
	{
		auto shot = make_shared<BlasterBurst>(pos, "BlasterShotMaterial", 0, speed);
		_shotList.push_back(shot);
	}

	void Space::PreloadModels()
	{
		Vector3 zero;

		for (auto i = 1; i <= 6; i++)
		{
			auto * asteroid = new Asteroid(zero, Asteroid::getAsteroidName(i), 0, 0);
			delete asteroid;
		}
		auto * enemyFighter = new EnemyFighter(zero, "ship.mesh", "ShipMaterialYellow", 0);
		delete enemyFighter;

		BlasterBurst * blasterBurst = new BlasterBurst(zero, "BlasterShotMaterial", 0, 80);
		blasterBurst->Destroy();
		blasterBurst->SetVisible(false);
		_shotList.push_back(shared_ptr<BlasterBurst>(blasterBurst));

		Sparks * sparks = new Sparks(zero);
		sparks->Destroy();
		sparks->SetVisible(false);
		_sparksList.push_back(shared_ptr<Sparks>(sparks));

		auto * orb = new EnergyOrb(zero);
		orb->Destroy();
		orb->SetVisible(false);
		_orbList.push_back(shared_ptr<EnergyOrb>(orb));

		Explosion * explosion = new Explosion(zero);
		explosion->Destroy();
		explosion->SetVisible(false);
		_explosionList.push_back(shared_ptr<Explosion>(explosion));

		auto * barrier = new Barrier(zero);
		barrier->Destroy();
		barrier->SetVisible(false);
		_barrierList.push_back(shared_ptr<Barrier>(barrier));
	}

	void Space::RegisterShotEvent(EventCallback callback)
	{
		_shotCallback = callback;
	}

	Level* Space::GetCurrentLevel()
	{
		return _currentLevel.get();
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

				auto * asteroid = new Asteroid(
						Vector3(-ASTEROID_NUM * BLOCK_SIZE, 0, pos),
						Asteroid::getAsteroidName(rand() % 6 + 1),
						0.0f,
						5.0f);
				_asteroidList.push_back(shared_ptr<Asteroid>(asteroid));
			}
			_lastObstacleCreated = totalTime;
		}
	}

	void Space::AddEnemyFighter(float totalTime)
	{
		if (totalTime > 3 && totalTime - _lastObstacleCreated > 9)
		{
			int posIndex = rand() % 3;
			float pos = presetPos[posIndex];

			auto fighter = make_shared<EnemyFighter>(Vector3(-ASTEROID_NUM * BLOCK_SIZE, 0, pos), "ship.mesh", "ShipMaterialYellow", 0.2f, 8.0f);
			_fighterList.push_back(fighter);

			_lastObstacleCreated = totalTime + 10;
		}
	}

	void Space::AddEnergyOrbs(float totalTime)
	{
		if (totalTime > 3 && totalTime - _lastObstacleCreated > 2.0f)
		{
			float pos;
			if (rand() % 3 == 0 && totalTime - _lastPosChange > 10)
			{
				if (_lastObstaclePos == 0 
					|| (rand() % 2 == 0 && _lastObstaclePos != 2))
				{
					pos = presetPos[_lastObstaclePos];
					pos += (presetPos[_lastObstaclePos + 1] - presetPos[_lastObstaclePos]) * 0.5f;
					_lastObstaclePos++;
				}
				else
				{
					pos = presetPos[_lastObstaclePos];
					pos -= (presetPos[_lastObstaclePos] - presetPos[_lastObstaclePos - 1]) * 0.5f;
					_lastObstaclePos--;
				}

				_lastPosChange = totalTime;
			}
			else 
			{
				pos = presetPos[_lastObstaclePos];
			}

			_orbList.push_back(make_shared<EnergyOrb>(Vector3(-ASTEROID_NUM * BLOCK_SIZE, 0, pos)));
			_lastObstacleCreated = totalTime;
		}
	}

	void Space::AddEnergyBarrier(float totalTime)
	{
		if (totalTime > 3 && totalTime - _lastObstacleCreated > 9)
		{
			auto barrier = make_shared<Barrier>(Vector3(-ASTEROID_NUM * BLOCK_SIZE, 0, 0));
			_barrierList.push_back(barrier);

			_lastObstacleCreated = totalTime + 6;
		}
	}

	void Space::UpdateShots(float time, float roadOffset)
	{
		for_each(_shotList.begin(), _shotList.end(), bind(&BlasterBurst::Update, placeholders::_1, time, roadOffset));
		
		for (auto& shot : _shotList)
		{
			for (auto& fighter : _fighterList)
			{
				if (!fighter->IsDone() && shot->IsIntersected(fighter.get()))
				{
					shot->Destroy();
					fighter->Destroy();
					_explosionList.push_back(make_shared<Explosion>(fighter->getPos()));
					if (_shotCallback)
						_shotCallback(SpaceObjectType::EnemyFighter);
				}
			}

			for (auto& asteroid : _asteroidList)
			{
				if (shot->IsIntersected(asteroid.get()))
				{
					shot->Destroy();
					_sparksList.push_back(make_shared<Sparks>(shot->getPos()));
					if (_shotCallback)
						_shotCallback(SpaceObjectType::Asteroid);
				}
			}

			for (auto& explosion : _explosionList)
			{
				if (shot->IsIntersected(explosion.get()))
				{
					shot->Destroy();
				}
			}
		}

		_shotList.erase(remove_if(_shotList.begin(), _shotList.end(), bind(&BlasterBurst::IsDone, placeholders::_1)), _shotList.end());
	}

	void Space::SetVisible(bool visible)
	{
		for_each(_asteroidList.begin(), _asteroidList.end(), bind(&Asteroid::SetVisible, placeholders::_1, visible));
		for_each(_fighterList.begin(), _fighterList.end(), bind(&EnemyFighter::SetVisible, placeholders::_1, visible));
		for_each(_shotList.begin(), _shotList.end(), bind(&BlasterBurst::SetVisible, placeholders::_1, visible));
		for_each(_orbList.begin(), _orbList.end(), bind(&EnergyOrb::SetVisible, placeholders::_1, visible));
		for_each(_explosionList.begin(), _explosionList.end(), bind(&Explosion::SetVisible, placeholders::_1, visible));
		for_each(_barrierList.begin(), _barrierList.end(), bind(&Barrier::SetVisible, placeholders::_1, visible));
		for_each(_backgroundAsteroidList.begin(), _backgroundAsteroidList.end(), bind(&Asteroid::SetVisible, placeholders::_1, visible));
		_fence->SetVisible(visible);
		_spaceDust->SetVisible(visible);
	}

	SpaceObjectType Space::IsIntersected(float turn)
	{
		for (auto i = _asteroidList.begin(); i != _asteroidList.end(); i++)
		{
			if ((*i)->IsIntersected(turn))
			{
				(*i)->Destroy();
				return SpaceObjectType::Asteroid;
			}
		}

		for (auto i = _fighterList.begin(); i != _fighterList.end(); i++)
		{
			if ((*i)->IsIntersected(turn))
			{
				(*i)->Destroy();
				return SpaceObjectType::EnemyFighter;
			}
		}

		for (auto i = _orbList.begin(); i != _orbList.end(); i++)
		{
			if ((*i)->IsIntersected(turn))
			{
				(*i)->Destroy();
				return SpaceObjectType::EnergyOrb;
			}
		}

		for (auto i = _barrierList.begin(); i != _barrierList.end(); i++)
		{
			if ((*i)->IsIntersected(turn))
			{
				return SpaceObjectType::Barrier;
			}
		}
		return SpaceObjectType::None;
	}
}