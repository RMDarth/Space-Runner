#pragma once
#include "Basic.h"
#include "SpaceDefs.h"
#include "SpaceObject.h"
#include <string>

class SceneSector;
class ModelDrawable;

namespace CoreEngine
{
	class Asteroid : public SpaceObject
	{
		ModelDrawable * _model;
		float _scale;
        Vector3 _size;

	public:
		Asteroid(Vector3 offset, std::string model, float speed, float scale = 1.0f);
		~Asteroid();
		void Update(float time, float roadSpeed) override;
		void TryReset();

        static std::string getAsteroidName(int num);

		void SetVisible(bool visible) override;
        void SetMaterial(std::string material);
        bool IsDone() override;

    protected:
        const Vector3& getHalfSize();
    };
}