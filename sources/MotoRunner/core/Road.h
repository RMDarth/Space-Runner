#pragma  once
#include <vector>
#include <memory>


#define VISIBLE_NUM 15
#define BUFFER_NUM 25

namespace CoreEngine
{
	class RoadLine;
	class Car;

	class Road
	{
		std::vector<RoadLine*> _roadLineList;
		std::vector<std::shared_ptr<Car>> _carList;
		RoadLine * _last;
		
		float _totalTime;
		float _lastCarCreated;
	public:
		Road();
		void GenerateRoad();
		void Update(float time, float speed);
		void SetVisible(bool visible);
		bool IsIntersected(float turn);
	private:
		void AddCars(float time);
	};
}