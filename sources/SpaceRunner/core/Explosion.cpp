#include "Explosion.h"
#include "SceneSector.h"
#include "RenderProcessor.h"
#include "ParticleSystem.h"
#include <sstream>
#define EXPLOSIONS_NUM 4

namespace CoreEngine
{
	const float Explosion::_explosionTime[4] = { 1.0f, 2.0f, 1.5f, 2.0f };;

	Explosion::Explosion(Vector3 offset)
		: SpaceObject(offset, 0)
	{
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		auto sceneNode = sceneManager->createSceneNode();
		sceneManager->getRootSceneNode()->addChild(sceneNode);
		_sector = new SceneSector(sceneNode);
		sceneNode->setPosition(VectorToOgre(offset));

		for (auto i = 0; i < EXPLOSIONS_NUM; i++)
		{
			auto sceneNodeChild = sceneManager->createSceneNode();
			sceneNode->addChild(sceneNodeChild);
			std::stringstream str;
			str << "Blast" << (i + 1) << "_%d";
			std::string templateName = str.str();
			str.str("");
			str << "Blast" << (i + 1);
			_explosionEffect[i] = new ParticleSystem(sceneNodeChild, templateName, str.str(), _explosionTime[i], EXPLOSIONS_NUM - i, true);
		}
	}

	Explosion::~Explosion()
	{
		for (int i = 0; i < EXPLOSIONS_NUM; i++)
			delete _explosionEffect[i];
	}

	void Explosion::Update(float time, float roadSpeed)
	{
		SpaceObject::Update(time, roadSpeed);
		_sector->GetNode()->setPosition(VectorToOgre(_pos));
		for (int i = 0; i < EXPLOSIONS_NUM; i++)
		{
			_explosionEffect[i]->Update(time);
		}
	}

	bool Explosion::IsDone()
	{
		return _explosionEffect[1]->IsFinished();
	}

	void Explosion::SetVisible(bool visible)
	{
		_sector->GetNode()->setVisible(visible);
	}

}