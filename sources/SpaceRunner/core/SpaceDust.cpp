#include "SpaceDust.h"
#include "ModelDrawable.h"
#include "SceneSector.h"
#include "RenderProcessor.h"

using namespace std;

namespace CoreEngine
{
	SpaceDust::SpaceDust(std::string particle)
	{
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		auto sceneNode1 = sceneManager->createSceneNode();
		sceneManager->getRootSceneNode()->addChild(sceneNode1);
		_sector1 = make_unique<SceneSector>(sceneNode1);

		auto sceneNode2 = sceneManager->createSceneNode();
		sceneManager->getRootSceneNode()->addChild(sceneNode2);
		_sector2 = make_unique<SceneSector>(sceneNode2);
		
		_sector2->GetNode()->setPosition(Ogre::Vector3(-BLOCK_SIZE * 10, 0, 0));

		_pos1 = Vector3(0, 0, 0);
		_pos2 = Vector3(-BLOCK_SIZE * 15, 0, 0);

		Generate(std::move(particle));
	}


	SpaceDust::~SpaceDust()
	{

	}

	void SpaceDust::Generate(std::string material)
	{
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		for (auto i = 0; i < SPACE_DUST_NUM; i++)
		{
			auto sceneNode = sceneManager->createSceneNode();
			_sector1->GetNode()->addChild(sceneNode);
			auto posX = (rand() % (int)(BLOCK_SIZE * 5.0f * 10)) / 10.0f - BLOCK_SIZE * 2.5f;
			auto posY = (rand() % (int)(BLOCK_SIZE * 5.0f * 10)) / 10.0f - BLOCK_SIZE * 2.5f;
			auto posZ = (rand() % BLOCK_SIZE * 15);
			sceneNode->setPosition(Ogre::Vector3(-posZ, posY, posX));
			auto particle = make_shared<ParticleDrawable>(new SceneSector(sceneNode), material, 0.3f);
			_particleList1.push_back(particle);
		}
		for (auto i = 0; i < SPACE_DUST_NUM; i++)
		{
			auto sceneNode = sceneManager->createSceneNode();
			_sector2->GetNode()->addChild(sceneNode);
			auto posX = (rand() % (int)(BLOCK_SIZE * 5.0f * 10)) / 10.0f - BLOCK_SIZE * 2.5f;
			auto posY = (rand() % (int)(BLOCK_SIZE * 5.0f * 10)) / 10.0f - BLOCK_SIZE * 2.5f;
			auto posZ = (rand() % BLOCK_SIZE * 15);
			sceneNode->setPosition(Ogre::Vector3(-posZ, posY, posX));
			auto particle = make_shared<ParticleDrawable>(new SceneSector(sceneNode), material, 0.3f);
			_particleList2.push_back(particle);
		}
	}
	
	void SpaceDust::Update(float time, float roadSpeed)
	{
		_pos1.x += roadSpeed;
		_pos2.x += roadSpeed;
		
		if (_pos1.x > BLOCK_SIZE * 15)
		{
			_pos1.x = -BLOCK_SIZE * 15;
		}
		if (_pos2.x > BLOCK_SIZE * 15)
		{
			_pos2.x = -BLOCK_SIZE * 15;
		}

		_sector1->GetNode()->setPosition(VectorToOgre(_pos1));
		_sector2->GetNode()->setPosition(VectorToOgre(_pos2));
	}

	void SpaceDust::SetVisible(bool visible)
	{
		_sector1->GetNode()->setVisible(visible);
		_sector2->GetNode()->setVisible(visible);
	}

}