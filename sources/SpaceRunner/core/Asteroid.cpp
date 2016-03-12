#include "Asteroid.h"
#include "ModelDrawable.h"
#include "SceneSector.h"
#include "RenderProcessor.h"

namespace CoreEngine
{
	Asteroid::Asteroid(Vector3 offset, std::string model, float speed)
	{
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		auto sceneNode = sceneManager->createSceneNode();
		sceneManager->getRootSceneNode()->addChild(sceneNode);
		_sector = new SceneSector(sceneNode);
		sceneNode->setPosition(VectorToOgre(offset));

		_sector->GetNode()->setDirection(Ogre::Vector3(1, 0, 0));
		_model = new ModelDrawable(_sector, model);
		//_model->SetScale(1);
		_speed = speed;
		_pos = offset;
	}

	Asteroid::~Asteroid()
	{
		delete _model;
		delete _sector;
	}

	void Asteroid::Update(float time, float roadSpeed)
	{
		_pos.x -= _speed * time;
		_pos.x += roadSpeed;
		_sector->GetNode()->setPosition(VectorToOgre(_pos));
	}

	bool Asteroid::IsIntersected(float turn)
	{
		float absPos = _pos.x;
		if (absPos < 12 && absPos > 8 && _pos.z > turn - 1.2 && _pos.z < turn + 1.2f)
		{
			return true;
		}
		return false;
	}

	bool Asteroid::IsDone()
	{
		if (_pos.x > BLOCK_SIZE * 5)
			return true;
		return false;
	}

	void Asteroid::TryReset()
	{
		if (_pos.x > BLOCK_SIZE * 5)
		{
			float posX = 0;
			while (posX > -BLOCK_SIZE*1.5f && posX < BLOCK_SIZE*1.5f)
			{
				posX = (rand() % (int)(BLOCK_SIZE * 10.0f * 10)) / 10.0f - BLOCK_SIZE * 5.0f;
			}
			//auto posX = (rand() % (int)(BLOCK_SIZE * 10.0f * 10)) / 10.0f - BLOCK_SIZE * 5.0f;
			auto posY = (rand() % (int)(BLOCK_SIZE * 10.0f * 10)) / 10.0f - BLOCK_SIZE * 5.0f;
			_pos = Vector3(-ASTEROID_NUM * BLOCK_SIZE, posY, posX);
			_sector->GetNode()->setPosition(VectorToOgre(_pos));
			_model->SetScale((rand()%15 + 5) / 10.0f);
		}
	}

	void Asteroid::SetVisible(bool visible)
	{
		_sector->GetNode()->setVisible(visible);
	}
}