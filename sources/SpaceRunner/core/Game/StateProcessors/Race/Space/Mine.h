#pragma once
#include "Basic.h"
#include "SpaceDefs.h"
#include "SpaceObject.h"
#include <string>

class SceneSector;
class ModelDrawable;

namespace CoreEngine
{
	class Mine : public SpaceObject
	{
		ModelDrawable * _model;

	public:
		Mine(Vector3 offset, float speed, float scale = 1.0f);
		~Mine();
		void Update(float time, float roadSpeed) override;
		void SetVisible(bool visible) override;
	};
}