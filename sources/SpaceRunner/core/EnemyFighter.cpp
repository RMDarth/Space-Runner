#include "EnemyFighter.h"
#include "ModelDrawable.h"
#include "SceneSector.h"
#include "RenderProcessor.h"

namespace CoreEngine
{
	EnemyFighter::EnemyFighter(Vector3 offset, std::string model, std::string material, float speed, float scale)
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

		_speed = speed;
		_pos = offset;
	}

	EnemyFighter::~EnemyFighter()
	{
		delete _model;
		delete _sector;
	}

	void EnemyFighter::Update(float time, float roadSpeed)
	{
		_pos.x -= _speed * time;
		_pos.x += roadSpeed;
		_sector->GetNode()->setPosition(VectorToOgre(_pos));
	}

	bool EnemyFighter::IsIntersected(float turn)
	{
		float absPos = _pos.x;
		if (absPos < 12 && absPos > 8 && _pos.z > turn - 1.2 && _pos.z < turn + 1.2f)
		{
			return true;
		}
		return false;
	}

	bool EnemyFighter::IsDone()
	{
		if (_pos.x > BLOCK_SIZE * 5)
			return true;
		return false;
	}

	void EnemyFighter::SetVisible(bool visible)
	{
		_sector->GetNode()->setVisible(visible);
	}
}