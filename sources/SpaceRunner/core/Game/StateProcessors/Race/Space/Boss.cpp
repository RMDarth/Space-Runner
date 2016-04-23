#include "Boss.h"
#include "Render/Drawables/ModelDrawable.h"

#define PI 3.1415f

namespace CoreEngine
{
    static const float presetPos[3] = { -BLOCK_SIZE * 1.1f, 0, BLOCK_SIZE * 1.1f };

    Boss::Boss(Vector3 offset, int lives)
            : SpaceObject(offset, 0)
            , _lives(lives)
    {
        auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
        auto sceneNode = sceneManager->createSceneNode();
        sceneManager->getRootSceneNode()->addChild(sceneNode);
        _sector = new SceneSector(sceneNode);
        sceneNode->setPosition(VectorToOgre(offset));

        _sector->GetNode()->setDirection(Ogre::Vector3(0, 0, 1));
        _model = new ModelDrawable(_sector, "ship.mesh");
        _model->SetMaterial("ShipMaterialYellow");
        _model->SetScale(7.0f);

        _posChangingStarted = -1;
        _totalTime = 0;
        _currentPosID = 1;
        _targetPosID = 1;
    }

    Boss::~Boss()
    {
        delete _model;
        delete _sector;
    }

    void Boss::Update(float time, float roadSpeed)
    {
        _totalTime += time;
        if (_pos.x < -20.0f)
        {
            SpaceObject::Update(time, roadSpeed);

            // roadSpeed = time * speed * 5;
            float shipSpeed = roadSpeed / (time * 5);

            if (_pos.x > -40)
                _speed = (1 - (_pos.x + 20) / -20) * (shipSpeed - 1) * 5.0f;

            if (_pos.x > -20.0f)
            {
                _pos.x = -20.0f;
                _posChangingStarted = _totalTime;
            }
        }

        if (_posChangingStarted > 0 && _totalTime - _posChangingStarted > 1.5f)
        {
            while (_targetPosID == _currentPosID)
                _targetPosID = rand() % 3;
            _posChangingStarted = _totalTime;
            _posChangingTime = 0.4f * abs(_targetPosID - _currentPosID);
        }

        UpdateTurn();
        _sector->GetNode()->setPosition(VectorToOgre(_pos));
    }

    void Boss::SetVisible(bool visible)
    {
        _sector->GetNode()->setVisible(visible);
    }

    void Boss::UpdateTurn()
    {
        if (_currentPosID != _targetPosID)
        {
            auto curTime = _totalTime - _posChangingStarted;
            if (curTime > _posChangingTime)
            {
                _pos.z = presetPos[_targetPosID];
                _currentPosID = _targetPosID;
                _angleHorizontal = 0;
                return;
            }

            auto scaleTime = curTime * PI / _posChangingTime;

            auto k = 0.5f * (sin(scaleTime - PI*0.5f) + 1);
            _pos.z = static_cast<float>(presetPos[_currentPosID] + k * (presetPos[_targetPosID] - presetPos[_currentPosID]));

            auto sign = _currentPosID < _targetPosID ? 1.0f : -1.0f;
            _angleHorizontal = static_cast<float>(sin(scaleTime) * _angleHorizontalMax * sign);
        }
    }

    void Boss::Hit()
    {
        _lives--;
    }

    bool Boss::IsDone()
    {
        return SpaceObject::IsDone() || _lives <= 0;
    }

    int Boss::GetLives()
    {
        return _lives;
    }


}