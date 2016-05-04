#pragma once
#include "Basic.h"
#include "SpaceDefs.h"
#include "SpaceObject.h"
#include <string>

class SceneSector;
class ModelDrawable;
class RectDrawable;
class ParticleSystem;

namespace CoreEngine
{
	class BlasterBurst : public SpaceObject
	{
		UPtr<RectDrawable> _rect;

		ParticleSystem * _glowEffect;

	public:
		BlasterBurst(Vector3 offset, std::string material, float speed);
		~BlasterBurst();
		void Update(float time, float roadSpeed) override;
		
		bool IsDone() override;
		void SetVisible(bool visible) override;

	private:
		const Vector3& getHalfSize() override;

	};
}