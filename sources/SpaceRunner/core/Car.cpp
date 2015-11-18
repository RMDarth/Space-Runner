#include "Car.h"
#include "ModelDrawable.h"
#include "SceneSector.h"
#include "RenderProcessor.h"
#include "RoadBlock.h"

namespace CoreEngine
{
	Car::Car(Vector3 offset, std::string model, float speed)
	{
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		auto sceneNode = sceneManager->createSceneNode();
		sceneManager->getRootSceneNode()->addChild(sceneNode);
		_sector = new SceneSector(sceneNode);
		sceneNode->setPosition(VectorToOgre(offset));

		_sector->GetNode()->setDirection(Ogre::Vector3(1, 0, 0));
		_model = new ModelDrawable(_sector, model);
		_model->SetScale(25);
		_speed = speed;
		_pos = offset;
	}

	Car::~Car()
	{
		delete _model;
		delete _sector;
	}

	void Car::Update(float time, float roadSpeed)
	{
		_pos.x -= _speed * time;
		_pos.x += roadSpeed;
		_sector->GetNode()->setPosition(VectorToOgre(_pos));
	}

	bool Car::IsIntersected(float turn)
	{
		float absPos = _pos.x;
		if (absPos < 12 && absPos > 8 && _pos.z > turn - 1.2 && _pos.z < turn + 1.2f)
		{
			return true;
		}
		return false;
	}

	bool Car::IsDone()
	{
		if (_pos.x > ROADBLOCK_SIZE * 5)
			return true;
		return false;
	}

	void Car::SetVisible(bool visible)
	{
		_sector->GetNode()->setVisible(visible);
	}
}