#include "Missile.h"
#include "Render/Drawables/ModelDrawable.h"
#include "Render/ParticleSystem.h"

namespace CoreEngine
{
    Missile::Missile(Vector3 offset, float speed, float scale)
            : SpaceObject(offset, speed)
    {
        auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
        auto sceneNode = sceneManager->createSceneNode();
        sceneManager->getRootSceneNode()->addChild(sceneNode);
        _sector = new SceneSector(sceneNode);

        sceneNode->lookAt(Ogre::Vector3(2,0,0), Ogre::Node::TransformSpace::TS_LOCAL);
        sceneNode->setPosition(VectorToOgre(offset));

        _sector->GetNode()->setDirection(Ogre::Vector3(0, -1, 0));
        _model = new ModelDrawable(_sector, "Missile.mesh");
        _model->SetScale(scale);

        auto sceneNodeChild = sceneManager->createSceneNode();
        sceneNode->addChild(sceneNodeChild);
        _smokeEffect = new ParticleSystem(sceneNodeChild, "missileSmoke_%d", "MissileSmoke", 100, 0, true);

        auto sceneNodeChild2 = sceneManager->createSceneNode();
        sceneNode->addChild(sceneNodeChild2);
        _fireEffect = new ParticleSystem(sceneNodeChild2, "missileFire_%d", "MissileFire", 100, 1, true);

    }

    Missile::~Missile()
    {
        delete _model;
        delete _smokeEffect;
        delete _fireEffect;
        delete _sector;
    }

    void Missile::Update(float time, float roadSpeed)
    {
        SpaceObject::Update(time, roadSpeed);
        _sector->GetNode()->setPosition(VectorToOgre(_pos));
        _smokeEffect->Update(time);
        _fireEffect->Update(time);
    }

    void Missile::SetVisible(bool visible)
    {
        _sector->GetNode()->setVisible(visible);
    }
}