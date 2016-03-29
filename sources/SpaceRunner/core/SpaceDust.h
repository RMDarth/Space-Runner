#pragma once
#include "Basic.h"
#include "GameDefs.h"
#include "ParticleDrawable.h"
#include "SpaceDefs.h"
#include <string>

#define SPACE_DUST_NUM 100

class SceneSector;
class ModelDrawable;

namespace CoreEngine
{
	class SpaceDust
	{
		std::unique_ptr<SceneSector> _sector1;
		std::unique_ptr<SceneSector> _sector2;
		PtrList<ParticleDrawable> _particleList1;
		PtrList<ParticleDrawable> _particleList2;

		Vector3 _pos1;
		Vector3 _pos2;

	public:
		SpaceDust(std::string particle);
		~SpaceDust();
		void Generate(std::string particle);
		void Update(float time, float roadSpeed);
		void SetVisible(bool visible);

	};
}