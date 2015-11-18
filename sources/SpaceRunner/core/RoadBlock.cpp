#include "RoadBlock.h"
#include "ModelDrawable.h"
#include "RectDrawable.h"
#include "SceneSector.h"
#include "RoadLine.h"
#include <sstream>

namespace CoreEngine
{
	RoadBlock::RoadBlock(SceneSector* parent, Vector3 pos)
	{
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		auto sceneNode = sceneManager->createSceneNode();
		sceneNode->setPosition(VectorToOgre(pos));
		parent->GetNode()->addChild(sceneNode);
		_sector = new SceneSector(sceneNode);

		_specialModel = nullptr;

		std::vector<Vector3> pointList;
		pointList.push_back(Vector3(-ROADBLOCK_SIZE * 0.5f, 0, -ROADBLOCK_SIZE * 0.5f));
		pointList.push_back(Vector3(ROADBLOCK_SIZE * 0.501f, 0, -ROADBLOCK_SIZE * 0.5f));
		pointList.push_back(Vector3(ROADBLOCK_SIZE * 0.501f, 0, ROADBLOCK_SIZE * 0.5f));
		pointList.push_back(Vector3(-ROADBLOCK_SIZE * 0.5f, 0, ROADBLOCK_SIZE * 0.5f));
		_roadModel = new RectDrawable(_sector, "RoadMaterial1", pointList);
	}

	RoadBlock::~RoadBlock()
	{
		if (_specialModel != nullptr)
			delete _specialModel;
		delete _roadModel;
		delete _sector;
	}

	void RoadBlock::Init(int tile, std::string specialModel /* = "" */, bool blocked /* = false */)
	{
		_blocked = blocked;
		if (_specialModel != nullptr)
			delete _specialModel;
		
		if (specialModel != "")
			_specialModel = new ModelDrawable(_sector, specialModel);
		else
			_specialModel = nullptr;

		std::stringstream ss;
		ss << "RoadMaterial" << tile;
		_roadModel->SetMaterial(ss.str());
	}

	void RoadBlock::Update(float time)
	{
		if (_specialModel != nullptr)
			_specialModel->Update(time); 
	}
}