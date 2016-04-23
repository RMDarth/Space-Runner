#include "Sparks.h"
#include "Render/SceneSector.h"
#include "RenderProcessor.h"
#include "Render/ParticleSystem.h"

namespace CoreEngine
{
    Sparks::Sparks(Vector3 offset, float speedMultiplier)
        : SpaceObject(offset, 0)
        , _multiplier(speedMultiplier)
    {
        auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
        auto sceneNode = sceneManager->createSceneNode();

        sceneManager->getRootSceneNode()->addChild(sceneNode);

        _sector = new SceneSector(sceneNode);
        sceneNode->setPosition(VectorToOgre(offset));

        auto sceneNode2 = sceneManager->createSceneNode();
        sceneNode->addChild(sceneNode2);

        _glowEffect = new ParticleSystem(sceneNode, "ShotGlow_%d", "ShotGlow", 1.5f, 0, true);
        _sparksEffect = new ParticleSystem(sceneNode2, "ShotSparks_%d", "ShotSparks", 1.5f, 0, true);
    }

    Sparks::~Sparks()
    {
        delete _sparksEffect;
        delete _glowEffect;
    }

    void Sparks::Update(float time, float roadSpeed)
    {
        SpaceObject::Update(time, roadSpeed + roadSpeed * _multiplier);
        _sector->GetNode()->setPosition(VectorToOgre(_pos));

        _sparksEffect->Update(time);
        _glowEffect->Update(time);
    }

    bool Sparks::IsDone()
    {
        return _sparksEffect->IsFinished();
    }

    void Sparks::SetVisible(bool visible)
    {
        _sector->GetNode()->setVisible(visible);
    }




}