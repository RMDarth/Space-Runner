#pragma once
#include "Basic.h"
#include "SpaceDefs.h"
#include <string>

class SceneSector;
class ModelDrawable;

namespace CoreEngine
{
	class Asteroid
	{
		SceneSector * _sector;
		ModelDrawable * _model;
		float _speed;
		Vector3 _pos;

	public:
		Asteroid(Vector3 offset, std::string model, float speed);
		~Asteroid();
		void Update(float time, float roadSpeed);
		bool IsIntersected(float turn);
		bool IsDone();
		void TryReset();
		void SetVisible(bool visible);

	};
}