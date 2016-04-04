#include "Asteroid.h"
#include "Render/Drawables/ModelDrawable.h"

namespace CoreEngine
{
	Asteroid::Asteroid(Vector3 offset, std::string model, float speed, float scale)
		: SpaceObject(offset, speed)
	{
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		auto sceneNode = sceneManager->createSceneNode();
		sceneManager->getRootSceneNode()->addChild(sceneNode);
		_sector = new SceneSector(sceneNode);
		sceneNode->setPosition(VectorToOgre(offset));

		_sector->GetNode()->setDirection(Ogre::Vector3(1, 0, 0));
		_model = new ModelDrawable(_sector, model);
		_model->SetScale(scale);

		_scale = scale;

		const auto& size = SpaceObject::getHalfSize();
		_size = size * _scale * 0.2f;
	}

	Asteroid::~Asteroid()
	{
		delete _model;
		delete _sector;
	}

	void Asteroid::Update(float time, float roadSpeed)
	{
		SpaceObject::Update(time, roadSpeed);
		_sector->GetNode()->setPosition(VectorToOgre(_pos));
	}

	void Asteroid::TryReset()
	{
		if (_pos.x > BLOCK_SIZE * 5)
		{
			float posX = 0;
			while (posX > -BLOCK_SIZE*15.0f && posX < BLOCK_SIZE*15.0f)
			{
				posX = (rand() % (int)(BLOCK_SIZE * 50.0f * 10)) / 10.0f - BLOCK_SIZE * 25.0f;
			}

			auto posY = (rand() % (int)(BLOCK_SIZE * 40.0f * 10)) / 10.0f - BLOCK_SIZE * 20.0f;
			_pos = Vector3(-ASTEROID_NUM * BLOCK_SIZE * 10, posY, posX);
			_sector->GetNode()->setPosition(VectorToOgre(_pos));
			//_model->SetScale((rand() % 15 + 5) / 10.0f);
		}
	}

	void Asteroid::SetVisible(bool visible)
	{
		_sector->GetNode()->setVisible(visible);
	}

	const Vector3& Asteroid::getHalfSize()
	{
		return _size;
	}

	bool Asteroid::IsDone()
	{
		return _pos.x + getHalfSize().x > BLOCK_SIZE * 5 || _destroyed;
	}

	std::string Asteroid::getAsteroidName(int num)
	{
		std::stringstream ss;
		ss << "Asteroid" << num << "_LOD0.mesh";

		return ss.str();
	}

    void Asteroid::SetMaterial(std::string material)
    {
        _model->SetMaterial(material);
    }


}