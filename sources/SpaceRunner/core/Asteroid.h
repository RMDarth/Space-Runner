#pragma once
#include "Basic.h"
#include "SpaceDefs.h"
#include "SpaceObject.h"
#include <string>

class SceneSector;
class ModelDrawable;

namespace CoreEngine
{
	class Asteroid : public SpaceObject
	{
		SceneSector * _sector;
		ModelDrawable * _model;

	public:
		Asteroid(Vector3 offset, std::string model, float speed, float scale = 1.0f);
		~Asteroid();
		void Update(float time, float roadSpeed) override;
		void TryReset();

		void SetVisible(bool visible) override;

	};
}