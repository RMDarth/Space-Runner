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
		//AddAsteroids(_totalTime);
		for_each(_backgroundAsteroidList.begin(), _backgroundAsteroidList.end(), bind(&Asteroid::TryReset, placeholders::_1));
		for_each(_backgroundAsteroidList.begin(), _backgroundAsteroidList.end(), bind(&Asteroid::Update, placeholders::_1, time, roadOffset));
		//for_each(_asteroidList.begin(), _asteroidList.end(), bind(&Asteroid::Update, placeholders::_1, time, roadOffset));
		//_asteroidList.erase(remove_if(_asteroidList.begin(), _asteroidList.end(), bind(&Asteroid::IsDone, placeholders::_1)), _asteroidList.end());
	}

	void Space::AddAsteroids(float time)
	{
		if (time > 3 && time - _lastAsteroidCreated > 3)
		{
			vector<float> posList;
			int count = rand() % 7;
			count = count % 4;
			if (count == 0)
				count++;
			for (int i = 0; i < count; i++)
			{
				float pos;
				bool found;
				while (true) {
					found = false;
					pos = (rand() % (int)(BLOCK_SIZE * 2.6 * 10)) / 10.0f - BLOCK_SIZE * 1.3f;
					for (unsigned int r = 0; r < posList.size(); r++)
						if (fabs(pos - posList[r]) < 2.5f)
						{
							found = true;
							break;
						}
					if (!found)
						break;
				}
				posList.push_back(pos);

				Asteroid * asteroid = new Asteroid(Vector3(-ASTEROID_NUM * BLOCK_SIZE, 0, pos), "moon.mesh", 4.0f + ((rand() % 30) / 10 - 1) * 0.5f);
				_asteroidList.push_back(shared_ptr<Asteroid>(asteroid));
			}
			_lastAsteroidCreated = time;
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