#include "Space.h"
#include "SpaceDust.h"
#include "Fence.h"
#include "Asteroid.h"
#include "SceneSector.h"
#include <memory>
#include <algorithm>
#include <functional>
using namespace std;

namespace CoreEngine
{
	Space::Space()
	{
		_lastAsteroidCreated = 0;
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
	}

	void Space::Update(float time, float speed)
	{
		_totalTime += time * speed;
		float roadOffset = time * speed * 5.0f;

		_spaceDust->Update(time, roadOffset);
		_fence->Update(time, roadOffset);
		
		AddAsteroids(_totalTime);
		
		for_each(_backgroundAsteroidList.begin(), _backgroundAsteroidList.end(), bind(&Asteroid::TryReset, placeholders::_1));
		for_each(_backgroundAsteroidList.begin(), _backgroundAsteroidList.end(), bind(&Asteroid::Update, placeholders::_1, time, roadOffset));
		
		for_each(_asteroidList.begin(), _asteroidList.end(), bind(&Asteroid::Update, placeholders::_1, time, roadOffset));
		_asteroidList.erase(remove_if(_asteroidList.begin(), _asteroidList.end(), bind(&Asteroid::IsDone, placeholders::_1)), _asteroidList.end());
	}

	void Space::AddAsteroids(float totalTime)
	{
		if (totalTime > 3 && totalTime - _lastAsteroidCreated > 4)
		{
			bool posUsed[3] = { false, false, false };
			static const float presetPos[3] = { -BLOCK_SIZE * 1.1f, 0, BLOCK_SIZE * 1.1f };

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
			_lastAsteroidCreated = totalTime;
		}

	}

	void Space::SetVisible(bool visible)
	{
		for_each(_asteroidList.begin(), _asteroidList.end(), bind(&Asteroid::SetVisible, placeholders::_1, visible));
		for_each(_backgroundAsteroidList.begin(), _backgroundAsteroidList.end(), bind(&Asteroid::SetVisible, placeholders::_1, visible));
		_spaceDust->SetVisible(visible);
	}

	bool Space::IsIntersected(float turn)
	{
		for (auto i = _asteroidList.begin(); i != _asteroidList.end(); i++)
		{
			if ((*i)->IsIntersected(turn))
			{
				return true;
			}
		}
		return false;
	}

}