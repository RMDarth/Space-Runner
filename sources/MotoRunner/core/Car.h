#pragma once
#include "Basic.h"
#include <string>

class SceneSector;
class ModelDrawable;

namespace CoreEngine
{
	class Car
	{
		SceneSector * _sector;
		ModelDrawable * _model;
		float _speed;
		Vector3 _pos;

	public:
		Car(Vector3 offset, std::string model, float speed);
		~Car();
		void Update(float time, float roadSpeed);
		bool IsIntersected(float turn);
		bool IsDone();
		void SetVisible(bool visible);

	};
}