#pragma once
#include "Basic.h"
#include "SpaceDefs.h"
#include "SpaceObject.h"
#include <string>

class SceneSector;
class ModelDrawable;
class ParticleSystem;

namespace CoreEngine
{
	class Explosion : public SpaceObject
	{
		ParticleSystem * _explosionEffect[4];

		static const float _explosionTime[4];

	public:
		Explosion(Vector3 offset, bool lowSparks = false, float scale = 1.0f);
		~Explosion();
		void Update(float time, float roadSpeed) override;
		bool IsDone() override;

		void SetVisible(bool visible) override;

	};
}