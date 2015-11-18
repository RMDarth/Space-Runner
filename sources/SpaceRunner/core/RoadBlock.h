#pragma once
#include <string>
#include "Basic.h"

class ModelDrawable;
class RectDrawable;
class SceneSector;

#define ROADBLOCK_SIZE 5.0f

namespace CoreEngine
{
	class RoadBlock
	{
		bool _blocked;
		ModelDrawable * _specialModel;
		RectDrawable * _roadModel;
		SceneSector* _sector;
	public:
		RoadBlock(SceneSector* parent, Vector3 pos);
		~RoadBlock();
		void Init(int tile, std::string specialModel = "", bool blocked = false);

		bool Blocked()
		{
			return _blocked;
		}

		void Update(float time);
	};
}