#pragma once
#include "Basic.h"
#include "SpaceDefs.h"
#include "SpaceObject.h"
#include <string>

class SceneSector;
class ModelDrawable;

namespace CoreEngine
{
    class Cruiser : public SpaceObject
    {
        SceneSector * _sector;
        ModelDrawable * _model;

    public:
        Cruiser(Vector3 offset, float speed, float scale = 1.0f);
        ~Cruiser();
        void Update(float time, float roadSpeed) override;
        void SetVisible(bool visible) override;

    private:
        const Vector3& getHalfSize() override;
    };
}