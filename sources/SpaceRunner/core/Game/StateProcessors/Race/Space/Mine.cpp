#include "Mine.h"
#include "Render/Drawables/ModelDrawable.h"
#include "Render/SceneSector.h"
#include "RenderProcessor.h"

namespace CoreEngine
{
	Mine::Mine(Vector3 offset, float speed, float scale)
		: SpaceObject(offset, speed)
	{
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		auto sceneNode = sceneManager->createSceneNode();
		sceneManager->getRootSceneNode()->addChild(sceneNode);
		_sector = new SceneSector(sceneNode);
		sceneNode->setPosition(VectorToOgre(offset));

		_sector->GetNode()->setDirection(Ogre::Vector3(0, 1, 0));
		_model = new ModelDrawable(_sector, "Mine.mesh");
		//_model->SetMaterial(material);
		_model->SetScale(100.0f);
	}

	Mine::~Mine()
	{
		delete _model;
		delete _sector;
	}

	void Mine::Update(float time, float roadSpeed)
	{
		SpaceObject::Update(time, roadSpeed);
		_sector->GetNode()->setPosition(VectorToOgre(_pos));
	}

	void Mine::SetVisible(bool visible)
	{
		_sector->GetNode()->setVisible(visible);
	}
}