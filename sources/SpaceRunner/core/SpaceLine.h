#pragma once
#include "Basic.h"
#include "GameDefs.h"
#include <memory>
#include <vector>
#include <string>

class ModelDrawable;
class SceneSector;

namespace CoreEngine
{
	class SpaceLine
	{
		Vector3 position;
		unique_ptr<SceneSector> _sector;
		ModelList _backgroundObjects;
		// add space dust

	public:
		SpaceLine(Vector3 pos);
		~SpaceLine();
		void SetPosition(Vector3 position);
		Vector3 GetPosition();
		void SetVisible(bool visible);

		void Update(float time);
	};
}