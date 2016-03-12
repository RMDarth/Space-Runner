#pragma once
#include "Basic.h"
#include "GameDefs.h"
#include "SpaceDefs.h"
#include <string>

#define SPACE_DUST_NUM 100

class RectDrawable;
class SceneSector;
class ModelDrawable;

namespace CoreEngine
{
	class Fence
	{
		std::unique_ptr<SceneSector> _sector1;
		std::unique_ptr<SceneSector> _sector2;
		std::unique_ptr<RectDrawable> _modelLeft[2];
		std::unique_ptr<RectDrawable> _modelRight[2];

		Vector3 _pos1;
		Vector3 _pos2;

	public:
		Fence();
		~Fence();
		void Generate();
		void Update(float time, float roadSpeed);
		void SetVisible(bool visible);

	};
}