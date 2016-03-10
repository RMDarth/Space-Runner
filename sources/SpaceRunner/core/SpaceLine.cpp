#include "SpaceLine.h"
#include "RoadBlock.h"
#include "RenderProcessor.h"
#include "SceneSector.h"
#include "ModelDrawable.h"
#include <memory>
#include <functional>
using namespace std;

namespace CoreEngine
{
	SpaceLine::SpaceLine(Vector3 pos)
	{
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		auto sceneNode = sceneManager->createSceneNode();
		sceneManager->getRootSceneNode()->addChild(sceneNode);
		sceneNode->setPosition(pos.x, pos.y, pos.z);
		_sector = new SceneSector(sceneNode);

		_specialModel = nullptr;
		_fenceLeft = nullptr;
		_fenceRight = nullptr;
		_borderLeft = nullptr;
		_borderRight = nullptr;


		_roadBlockList[0] = new RoadBlock(_sector, Vector3(0, 0, -ROADBLOCK_SIZE));
		_roadBlockList[1] = new RoadBlock(_sector, Vector3());
		_roadBlockList[2] = new RoadBlock(_sector, Vector3(0, 0, ROADBLOCK_SIZE));

		auto fenceLeftSector = sceneManager->createSceneNode();
		sceneNode->addChild(fenceLeftSector);
		_fenceLeftSector = new SceneSector(fenceLeftSector);
		fenceLeftSector->setPosition(0, 0, ROADBLOCK_SIZE * 2.5f);

		auto fenceRightSector = sceneManager->createSceneNode();
		sceneNode->addChild(fenceRightSector);
		_fenceRightSector = new SceneSector(fenceRightSector);
		fenceRightSector->setPosition(0, 0, -ROADBLOCK_SIZE * 2.5f);

		auto borderLeftSector = sceneManager->createSceneNode();
		sceneNode->addChild(borderLeftSector);
		_borderLeftSector = new SceneSector(borderLeftSector);
		borderLeftSector->setPosition(0, 0, ROADBLOCK_SIZE * 1.75f);

		auto borderRightSector = sceneManager->createSceneNode();
		sceneNode->addChild(borderRightSector);
		_borderRightSector = new SceneSector(borderRightSector);
		borderRightSector->setPosition(0, 0, -ROADBLOCK_SIZE * 1.75f);

		SetFence();
		SetBorder();
	}

	SpaceLine::~SpaceLine()
	{
		_sector->GetNode()->detachAllObjects();
		_sector.reset();
	}

	void SpaceLine::SetFence(std::string fance /* = "fense.mesh" */)
	{
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		auto sceneNode = _sector->GetNode();

		if (_fenceLeft != nullptr) 
		{
			delete _fenceLeft;
			delete _fenceRight;
		}
		
		_fenceLeft = new ModelDrawable(_fenceLeftSector, fance);
		_fenceLeft->SetScale(Vector3(3.5, 2.0, 3.0));
		_fenceRight = new ModelDrawable(_fenceRightSector, fance);
		_fenceRight->SetScale(Vector3(3.5, 2.0, 3.0));
	}

	void SpaceLine::SetBorder(std::string border /* = "border.mesh" */)
	{
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		auto sceneNode = _sector->GetNode();

		if (_borderLeft != nullptr) 
		{
			delete _borderLeft;
			delete _borderRight;
		}

		_borderLeft = new ModelDrawable(_borderLeftSector, border);
		_borderRight = new ModelDrawable(_borderRightSector, border);
	}

	void SpaceLine::SetSpecialModel(std::string specialModel /* = "" */)
	{
		if (_specialModel != nullptr)
			delete _specialModel;

		if (specialModel != "")
			_specialModel = new ModelDrawable(_sector, specialModel);
		else
			_specialModel = nullptr;
	}

	void SpaceLine::SetPosition(Vector3 position)
	{
		_sector->GetNode()->setPosition(VectorToOgre(position));
	}

	Vector3 SpaceLine::GetPosition()
	{
		return VectorFromOgre(_sector->GetNode()->getPosition());
	}

	void SpaceLine::Update(float time)
	{
		if (_fenceLeft)
		{
			_fenceLeft->Update(time);
			_fenceRight->Update(time);
		}
		if (_borderLeft)
		{
			_borderLeft->Update(time);
			_borderRight->Update(time);
		}
		if (_specialModel)
		{
			_specialModel->Update(time);
		}

		for (int i = 0; i < BLOCKS_NUM; i++)
		{
			_roadBlockList[i]->Update(time);
		}

	}

	void SpaceLine::SetRoadBlock(int index, int roadTile, std::string specialModel, bool blocked)
	{
		_roadBlockList[index]->Init(roadTile, specialModel, blocked);
	}

	void SpaceLine::SetVisible(bool visible)
	{
		_sector->GetNode()->setVisible(visible);
	}

}