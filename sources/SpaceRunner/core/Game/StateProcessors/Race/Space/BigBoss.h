#pragma once
#include "Basic.h"
#include "SpaceDefs.h"
#include "SpaceObject.h"
#include "BossInterface.h"
#include <string>

class SceneSector;
class ModelDrawable;

namespace CoreEngine
{
    class BigBoss : public BossInterface
    {
        ModelDrawable * _model;

        ModelDrawable * _modelTurret[3];
        SceneSector* _sectorTurret[3];

        SceneSector* _sectorParticle;
        ParticleSystem * _engineEffect;


        float _totalTime;

        int _lives = 50;

    public:
        BigBoss(Vector3 offset, int lives = 50);
        ~BigBoss();
        void Update(float time, float roadSpeed) override;
        void SetVisible(bool visible) override;

        void Hit();
        int GetLives();
        Type GetType() override { return Type::Big; }

        bool IsDone() override;

    private:
        const Vector3& getHalfSize() override;
    };
}