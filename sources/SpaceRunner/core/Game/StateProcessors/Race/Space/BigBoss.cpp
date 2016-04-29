#include <Render/ParticleSystem.h>
#include "BigBoss.h"
#include "Render/Drawables/ModelDrawable.h"

#define PI 3.1415f

namespace CoreEngine
{
    BigBoss::BigBoss(Vector3 offset, int lives, Difficulty difficulty)
            : BossInterface(offset, difficulty)
            , _lives(lives)
    {
        auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
        auto sceneNode = sceneManager->createSceneNode();
        sceneManager->getRootSceneNode()->addChild(sceneNode);
        _sector = new SceneSector(sceneNode);
        sceneNode->setPosition(VectorToOgre(offset));

        _sector->GetNode()->setDirection(Ogre::Vector3(0, 1, 0));
        _model = new ModelDrawable(_sector, "Cruiser.mesh");
        _model->SetMaterial("Cruiser_green");
        _model->SetScale(2.0f);

        Ogre::Vector3 pos[] = {
                {0, 2.5f, -0.5f},
                {0, 0, -0.7f},
                {0, -2.3f, -1.0f}
        };
        for (auto i = 0; i < 3; i++)
        {
            auto turretNode = sceneManager->createSceneNode();
            sceneNode->addChild(turretNode);
            turretNode->setDirection(1, 0, 0, Ogre::Node::TransformSpace::TS_LOCAL, Ogre::Vector3::UNIT_Y);
            turretNode->setPosition(pos[i]);
            _sectorTurret[i] = new SceneSector(turretNode);
            _modelTurret[i] = new ModelDrawable(_sectorTurret[i], "CruiserTurret.mesh");
            _modelTurret[i]->SetScale(3.0f);
        }

        auto sceneNodeParticle = sceneManager->createSceneNode();
        sceneNodeParticle->setPosition(0, -3.8f, 0);
        sceneNode->addChild(sceneNodeParticle);
        _sectorParticle = new SceneSector(sceneNodeParticle);

        _engineEffect = new ParticleSystem(sceneNodeParticle, "EngineFireCruiser_%d", "EngineRight", 100, 0, true);

        _totalTime = 0;
    }

    BigBoss::~BigBoss()
    {
        delete _model;
        for (auto i = 0; i < 3; i++)
        {
            delete _modelTurret[i];
            delete _sectorTurret[i];
        }

        delete _engineEffect;
        delete _sectorParticle;

        delete _sector;
    }

    void BigBoss::Update(float time, float roadSpeed)
    {
        _totalTime += time;
        if (_pos.x < -30.0f)
        {
            SpaceObject::Update(time, roadSpeed);

            // roadSpeed = time * speed * 5;
            float shipSpeed = roadSpeed / (time * 5);

            if (_pos.x > -50)
                _speed = (1 - (_pos.x + 20) / -20) * (shipSpeed - 1) * 5.0f;

            if (_pos.x > -30.0f)
            {
                _pos.x = -30.0f;
            }

            _pos.z = _pos.x + 30;
        }

        _sector->GetNode()->setPosition(VectorToOgre(_pos));
    }

    void BigBoss::SetVisible(bool visible)
    {
        _sector->GetNode()->setVisible(visible);
    }

    void BigBoss::Hit()
    {
        _lives--;
    }

    bool BigBoss::IsDone()
    {
        return SpaceObject::IsDone() || _lives <= 0;
    }

    int BigBoss::GetLives()
    {
        return _lives;
    }

    const Vector3 &BigBoss::getHalfSize()
    {
        static Vector3 standardBBSize(2.1, 2, BLOCK_SIZE*1.5f);
        return standardBBSize;
    }


}