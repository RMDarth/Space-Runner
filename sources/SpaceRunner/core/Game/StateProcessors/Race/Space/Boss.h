#pragma once
#include "Basic.h"
#include "SpaceDefs.h"
#include "SpaceObject.h"
#include <string>

class SceneSector;
class ModelDrawable;

namespace CoreEngine
{
    class Boss : public SpaceObject
    {
        SceneSector * _sector;
        ModelDrawable * _model;

        float _totalTime;

        float _angleHorizontal;
        const float _angleHorizontalMax = 0.7f;

        int _currentPosID;
        int _targetPosID;
        float _posChangingStarted;
        float _posChangingTime = 0.4f;

        int _lives = 50;

    public:
        Boss(Vector3 offset, int lives = 50);
        ~Boss();
        void Update(float time, float roadSpeed) override;
        void SetVisible(bool visible) override;

        void Hit();
        int GetLives();

        bool IsDone() override;

    private:
        void UpdateTurn();
    };
}