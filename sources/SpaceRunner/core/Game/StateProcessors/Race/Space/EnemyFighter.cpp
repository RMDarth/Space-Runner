#include "EnemyFighter.h"
#include "Render/Drawables/ModelDrawable.h"
#include "Render/SceneSector.h"
#include "RenderProcessor.h"

namespace CoreEngine
{
	EnemyFighter::EnemyFighter(Vector3 offset, std::string model, std::string material, float speed, float scale)
		: SpaceObject(offset, speed)
	{
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		auto sceneNode = sceneManager->createSceneNode();
		sceneManager->getRootSceneNode()->addChild(sceneNode);
		_sector = new SceneSector(sceneNode);
		sceneNode->setPosition(VectorToOgre(offset));

		_sector->GetNode()->setDirection(Ogre::Vector3(0, 0, 1));
		_model = new ModelDrawable(_sector, model);
		_model->SetMaterial(material);
		_model->SetScale(scale);
	}

	EnemyFighter::~EnemyFighter()
	{
		delete _model;
		delete _sector;
	}

	void EnemyFighter::Update(float time, float roadSpeed)
	{
		SpaceObject::Update(time, roadSpeed);
		_sector->GetNode()->setPosition(VectorToOgre(_pos));
	}

	void EnemyFighter::SetVisible(bool visible)
	{
		_sector->GetNode()->setVisible(visible);
	}
}