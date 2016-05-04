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
	class Mine : public SpaceObject
	{
		ModelDrawable * _model;
		ParticleSystem * _glowEffects[6];

	public:
		Mine(Vector3 offset, float speed);
		~Mine();
		void Update(float time, float roadSpeed) override;
		void SetVisible(bool visible) override;
	};
}