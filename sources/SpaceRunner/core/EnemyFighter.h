#pragma once
#include "Basic.h"
#include "SpaceDefs.h"
#include <string>

class SceneSector;
class ModelDrawable;

namespace CoreEngine
{
	class EnemyFighter
	{
		SceneSector * _sector;
		ModelDrawable * _model;
		float _speed;
		Vector3 _pos;

	public:
		EnemyFighter(Vector3 offset, std::string model, std::string material, float speed, float scale = 1.0f);
		~EnemyFighter();
		void Update(float time, float roadSpeed);
		bool IsIntersected(float turn);
		bool IsDone();
		void SetVisible(bool visible);

	};
}