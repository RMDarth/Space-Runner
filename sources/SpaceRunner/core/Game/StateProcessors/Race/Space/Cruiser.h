#pragma once
#include "Basic.h"
#include "SpaceDefs.h"
#include "SpaceObject.h"
#include <string>
#include <Render/SceneSector.h>

class SceneSector;
class ModelDrawable;

namespace CoreEngine
{
    class Cruiser : public SpaceObject
    {
        SceneSector * _sector;
        ModelDrawable * _model;

        ModelDrawable * _modelTurret[3];
        SceneSector* _sectorTurret[3];

    public:
        Cruiser(Vector3 offset, float speed, float scale = 1.0f);
        ~Cruiser();
        void Update(float time, float roadSpeed) override;
        void SetVisible(bool visible) override;

    private:
        const Vector3& getHalfSize() override;
    };
}