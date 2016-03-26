#pragma once
#include "Basic.h"
#include "SpaceDefs.h"
#include "SpaceObject.h"
#include <string>

class SceneSector;
class ModelDrawable;
class RectDrawable;

namespace CoreEngine
{
	class BlasterBurst : public SpaceObject
	{
		SceneSector * _sector;
		UPtr<RectDrawable> _rect;

	public:
		BlasterBurst(Vector3 offset, std::string material, int count, float speed, float size = 1.0f);
		~BlasterBurst();
		void Update(float time, float roadSpeed) override;
		
		bool IsDone() override;
		void SetVisible(bool visible) override;

	};
}