#include "Barrier.h"
#include "Render/Drawables/ModelDrawable.h"
#include "Render/SceneSector.h"
#include "RenderProcessor.h"
#include "Render/Drawables/RectDrawable.h"

namespace CoreEngine
{

	Barrier::Barrier(Vector3 offset)
		: SpaceObject(offset, 0)
	{
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		auto sceneNode = sceneManager->createSceneNode();
		sceneManager->getRootSceneNode()->addChild(sceneNode);
		_sector = new SceneSector(sceneNode);
		sceneNode->setPosition(VectorToOgre(offset));

		for (auto i = 0; i < 4; i++)
		{
			auto sceneNodeChild = sceneManager->createSceneNode();
			sceneNode->addChild(sceneNodeChild);
			auto sector = std::make_shared<SceneSector>(sceneNodeChild);

			_modelList[i] = new ModelDrawable(sector.get(), "probe.mesh");
			_modelList[i]->SetScale(50.0f);

			
			sceneNodeChild->setPosition(0, 1.0f + (i % 2) * 2, i < 2 ? -BLOCK_SIZE*1.55f : BLOCK_SIZE*1.55f);
			sceneNodeChild->setDirection(Ogre::Vector3(0, i < 2 ? 1 : -1, 0));
			_sectorList.push_back(std::move(sector));
		}

		for (auto i = 0; i < 2; i++)
		{
			auto sceneNodeChild = sceneManager->createSceneNode();
			sceneNode->addChild(sceneNodeChild);
			auto sector = std::make_shared<SceneSector>(sceneNodeChild);

			std::vector<Vector3> pointList;
			pointList.push_back(Vector3(0, 1.5 + 2 * i, -BLOCK_SIZE * 1.5f));
			pointList.push_back(Vector3(0, 0.5 + 2 * i, -BLOCK_SIZE * 1.5f));
			pointList.push_back(Vector3(0, 0.5 + 2 * i, BLOCK_SIZE * 1.5f));
			pointList.push_back(Vector3(0, 1.5 + 2 * i, BLOCK_SIZE * 1.5f));
			_rectList[i] = new RectDrawable(sector.get(), "Lightning", pointList);

			_sectorList.push_back(std::move(sector));
		}

		_material = Ogre::MaterialManager::getSingleton().getByName("Lightning").get();
		_alpha = 0;
	}

	Barrier::~Barrier()
	{
		for (auto i = 0; i < 4; i++)
		{
			delete _modelList[i];
		}
		for (auto i = 0; i < 2; i++)
		{
			delete _rectList[i];
		}
		delete _sector;
	}

	void Barrier::Update(float time, float roadSpeed)
	{
		SpaceObject::Update(time, roadSpeed);
		_sector->GetNode()->setPosition(VectorToOgre(_pos));
		
		_alpha += time * 3;
		while (_alpha > 1.0f)
			_alpha -= 1.0f;

		_material->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("color", Ogre::ColourValue(1, 1, 1, _alpha));
	}

	void Barrier::SetVisible(bool visible)
	{
		_sector->GetNode()->setVisible(visible);
	}

	const Vector3& Barrier::getHalfSize()
	{
		static Vector3 standardBarrierSize(2, 2, BLOCK_SIZE*1.5f);
		return standardBarrierSize;
	}

}