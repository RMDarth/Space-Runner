#pragma once
#include "Basic.h"
#include "SpaceDefs.h"
#include "SpaceObject.h"
#include <string>

class SceneSector;
class ModelDrawable;

namespace CoreEngine
{
	class EnemyFighter : public SpaceObject
	{
		SceneSector * _sector;
		ModelDrawable * _model;

	public:
		EnemyFighter(Vector3 offset, std::string model, std::string material, float speed, float scale = 1.0f);
		~EnemyFighter();
		void Update(float time, float roadSpeed) override;
		void SetVisible(bool visible) override;
	};
}