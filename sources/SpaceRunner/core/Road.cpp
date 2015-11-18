#include "Road.h"
#include "RoadLine.h"
#include "RoadBlock.h"
#include "SceneSector.h"
#include "Car.h"
#include <memory>
#include <algorithm>
#include <functional>
using namespace std;

namespace CoreEngine
{
	Road::Road()
	{
		_lastCarCreated = 0;
		GenerateRoad();
	}

	void Road::GenerateRoad()
	{
		for (int i = 0; i < BUFFER_NUM; i++)
		{
			auto roadLine = new RoadLine(Vector3(-i*ROADBLOCK_SIZE, 0, 0));
			roadLine->SetRoadBlock(0, rand() % 3 + 1);
			roadLine->SetRoadBlock(1, rand() % 3 + 1);
			roadLine->SetRoadBlock(2, rand() % 3 + 1);

			_roadLineList.push_back(roadLine);
			_last = roadLine;
		}
	}

	void Road::Update(float time, float speed)
	{
		_totalTime += time * speed;
		float roadOffset = time * speed * 5.0f;
		// Endless road processing
		for (int i = 0; i < BUFFER_NUM; i++)
		{
			Vector3 pos = _roadLineList[i]->GetPosition();
			pos.x += roadOffset;
			_roadLineList[i]->SetPosition(pos);

			if (pos.x > ROADBLOCK_SIZE * 3)
			{
				_roadLineList[i]->SetRoadBlock(0, rand() % 3 + 1);
				_roadLineList[i]->SetRoadBlock(1, rand() % 3 + 1);
				_roadLineList[i]->SetRoadBlock(2, rand() % 3 + 1);
				_roadLineList[i]->SetPosition(Vector3(-(BUFFER_NUM - 3) * ROADBLOCK_SIZE + (pos.x - ROADBLOCK_SIZE * 3), 0, 0));
				_last = _roadLineList[i];
			}
		}

		AddCars(_totalTime);
		for_each(_roadLineList.begin(), _roadLineList.end(), bind(&RoadLine::Update, placeholders::_1, time));
		for_each(_carList.begin(), _carList.end(), bind(&Car::Update, placeholders::_1, time, roadOffset));
		_carList.erase(remove_if(_carList.begin(), _carList.end(), bind(&Car::IsDone, placeholders::_1)), _carList.end());
	}

	void Road::AddCars(float time)
	{
		if (time > 3 && time - _lastCarCreated > 3)
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
					pos = (rand() % (int)(ROADBLOCK_SIZE * 2.6 * 10)) / 10.0f - ROADBLOCK_SIZE * 1.3f;
					for (int r = 0; r < posList.size(); r++)
						if (fabs(pos - posList[r]) < 2.5f)
						{
							found = true;
							break;
						}
					if (!found)
						break;
				}
				posList.push_back(pos);

				Car * car = new Car(Vector3(_last->GetPosition().x, 0, pos), "policecar.mesh", 4.0f + ((rand()% 30) / 10  - 1) * 0.5f );
				_carList.push_back(shared_ptr<Car>(car));
			}
			_lastCarCreated = time;
		}

	}

	void Road::SetVisible(bool visible)
	{
		for_each(_roadLineList.begin(), _roadLineList.end(), bind(&RoadLine::SetVisible, placeholders::_1, visible));
		for_each(_carList.begin(), _carList.end(), bind(&Car::SetVisible, placeholders::_1, visible));
	}

	bool Road::IsIntersected(float turn)
	{
		for (auto i = _carList.begin(); i != _carList.end(); i++)
		{
			if ((*i)->IsIntersected(turn))
			{
				return true;
			}
		}
		return false;
	}

}