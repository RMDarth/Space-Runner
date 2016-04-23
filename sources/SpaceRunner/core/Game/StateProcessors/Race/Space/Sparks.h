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
    class Sparks : public SpaceObject
    {
        ParticleSystem * _sparksEffect;
        ParticleSystem * _glowEffect;

        bool _hasParent;

    public:
        Sparks(Vector3 offset, SpaceObject* parent = nullptr);
        ~Sparks();
        void Update(float time, float roadSpeed) override;
        bool IsDone() override;

        void SetVisible(bool visible) override;

    };
}