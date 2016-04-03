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
	class EnergyOrb : public SpaceObject
	{
		SceneSector * _sector;
		ParticleSystem * _sparksEffect;
		ParticleSystem * _glowEffect;

	public:
		EnergyOrb(Vector3 offset);
		~EnergyOrb();
		void Update(float time, float roadSpeed) override;

		void SetVisible(bool visible) override;

	};
}