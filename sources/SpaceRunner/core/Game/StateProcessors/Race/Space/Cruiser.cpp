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
    }

    Cruiser::~Cruiser()
    {
        delete _model;
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