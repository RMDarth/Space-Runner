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
    class Missile : public SpaceObject
    {
        SceneSector * _sector;
        ModelDrawable * _model;
        ParticleSystem * _smokeEffect;
        ParticleSystem * _fireEffect;
        float _scale;
        Vector3 _size;

    public:
        Missile(Vector3 offset, float speed, float scale = 1.0f);
        ~Missile();
        void Update(float time, float roadSpeed) override;
        void SetVisible(bool visible) override;
    };
}