#pragma once
#include "Basic.h"
#include "Game/GameDefs.h"
#include "SpaceDefs.h"
#include <string>

namespace Ogre
{
    class Material;
}

class RectDrawable;
class SceneSector;
class ModelDrawable;

namespace CoreEngine
{
	class Fence
	{
		std::unique_ptr<SceneSector> _sector;
		std::unique_ptr<RectDrawable> _modelLeft;
		std::unique_ptr<RectDrawable> _modelRight;

		Vector3 _pos;

		Ogre::Material* _material;
		float _alpha;

	public:
		Fence();
		~Fence();
		void Generate();
		void Update(float time, float roadSpeed);
		void SetVisible(bool visible);

	};
}