#pragma once
#include "Basic.h"
#include "SpaceDefs.h"
#include <string>

class SceneSector;
class ModelDrawable;
class RectDrawable;

namespace CoreEngine
{
	class BlasterBurst
	{
		SceneSector * _sector;
		UPtr<RectDrawable> _rect;
		float _speed;
		Vector3 _pos;

	public:
		BlasterBurst(Vector3 offset, std::string material, int count, float speed, float size = 1.0f);
		~BlasterBurst();
		void Update(float time, float roadSpeed);
		bool IsDone();
		void SetVisible(bool visible);

	};
}