#include "EnergyOrb.h"
#include "SceneSector.h"
#include "RenderProcessor.h"
#include "ParticleSystem.h"

namespace CoreEngine
{
	EnergyOrb::EnergyOrb(Vector3 offset)
		: SpaceObject(offset, 0)
	{
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		auto sceneNode = sceneManager->createSceneNode();
		sceneManager->getRootSceneNode()->addChild(sceneNode);
		_sector = new SceneSector(sceneNode);
		sceneNode->setPosition(VectorToOgre(offset));

		auto sceneNode2 = sceneManager->createSceneNode();
		sceneNode->addChild(sceneNode2);

		_glowEffect = new ParticleSystem(sceneNode, "Orb1_%d", "Orb1", 100.0f, 0, true);
		_sparksEffect = new ParticleSystem(sceneNode2, "Orb2_%d", "Orb2", 100.0f, 0, true);
	}

	EnergyOrb::~EnergyOrb()
	{
		delete _sparksEffect;
		delete _glowEffect;
	}

	void EnergyOrb::Update(float time, float roadSpeed)
	{
		SpaceObject::Update(time, roadSpeed);
		_sector->GetNode()->setPosition(VectorToOgre(_pos));
		_sparksEffect->Update(time);
		_glowEffect->Update(time);
	}

	void EnergyOrb::SetVisible(bool visible)
	{
		_sector->GetNode()->setVisible(visible);
	}

}