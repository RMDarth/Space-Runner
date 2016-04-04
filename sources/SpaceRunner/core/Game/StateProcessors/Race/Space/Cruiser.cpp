#include "Cruiser.h"
#include "Render/Drawables/ModelDrawable.h"
#include "Render/SceneSector.h"
#include "RenderProcessor.h"

namespace CoreEngine
{
    Cruiser::Cruiser(Vector3 offset, float speed, float scale)
            : SpaceObject(offset, speed)
    {
        auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
        auto sceneNode = sceneManager->createSceneNode();
        sceneManager->getRootSceneNode()->addChild(sceneNode);
        _sector = new SceneSector(sceneNode);
        sceneNode->setPosition(VectorToOgre(offset));

        _sector->GetNode()->setDirection(Ogre::Vector3(0, 1, 0));
        _model = new ModelDrawable(_sector, "Cruiser.mesh");
        //_model->SetMaterial(material);
        _model->SetScale(scale);
        _model->SetVisible(true);

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
            _modelTurret[i]->SetScale(scale * 1.5);
        }
    }

    Cruiser::~Cruiser()
    {
        delete _model;
        for (auto i = 0; i < 3; i++)
        {
            delete _modelTurret[i];
            delete _sectorTurret[i];
        }

        delete _sector;
    }

    void Cruiser::Update(float time, float roadSpeed)
    {
        SpaceObject::Update(time, roadSpeed);
        _sector->GetNode()->setPosition(VectorToOgre(_pos));
    }

    void Cruiser::SetVisible(bool visible)
    {
        _sector->GetNode()->setVisible(visible);
    }

    const Vector3& Cruiser::getHalfSize()
    {
        static Vector3 standardCruiserSize(2, 2, BLOCK_SIZE*1.5f);
        return standardCruiserSize;
    }
}