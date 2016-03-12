#include "Fence.h"
#include "SceneSector.h"
#include "RenderProcessor.h"
#include "RectDrawable.h"

using namespace std;

namespace CoreEngine
{
	Fence::Fence()
	{
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		auto sceneNode1 = sceneManager->createSceneNode();
		sceneManager->getRootSceneNode()->addChild(sceneNode1);
		_sector1 = make_unique<SceneSector>(sceneNode1);

		auto sceneNode2 = sceneManager->createSceneNode();
		sceneManager->getRootSceneNode()->addChild(sceneNode2);
		_sector2 = make_unique<SceneSector>(sceneNode2);

		_sector2->GetNode()->setPosition(Ogre::Vector3(-BLOCK_SIZE * 40, 0, 0));

		_pos1 = Vector3(0, 0, 0);
		_pos2 = Vector3(-BLOCK_SIZE * 40, 0, 0);

		Generate();
	}


	Fence::~Fence()
	{

	}

	void Fence::Generate()
	{
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();

		std::vector<Vector3> pointList;
		pointList.push_back(Vector3(0, 0, -BLOCK_SIZE * 1.5));
		pointList.push_back(Vector3(-BLOCK_SIZE * 40.01f, 0, -BLOCK_SIZE * 1.5));
		pointList.push_back(Vector3(-BLOCK_SIZE * 40.01f, 0.5f, -BLOCK_SIZE * 1.5));
		pointList.push_back(Vector3(0, 0.5f, -BLOCK_SIZE * 1.5));
		_modelLeft[0] = make_unique<RectDrawable>(_sector1.get(), "FenceMaterial", pointList);
		_modelLeft[1] = make_unique<RectDrawable>(_sector2.get(), "FenceMaterial", pointList);

		pointList.clear();
		pointList.push_back(Vector3(0, 0, BLOCK_SIZE * 1.5));
		pointList.push_back(Vector3(-BLOCK_SIZE * 40.01f, 0, BLOCK_SIZE * 1.5));
		pointList.push_back(Vector3(-BLOCK_SIZE * 40.01f, 0.5f, BLOCK_SIZE * 1.5));
		pointList.push_back(Vector3(0, 0.5f, BLOCK_SIZE * 1.5));
		_modelRight[0] = make_unique<RectDrawable>(_sector1.get(), "FenceMaterial", pointList);
		_modelRight[1] = make_unique<RectDrawable>(_sector2.get(), "FenceMaterial", pointList);
	}

	void Fence::Update(float time, float roadSpeed)
	{
		_pos1.x += roadSpeed;
		_pos2.x += roadSpeed;

		if (_pos1.x > BLOCK_SIZE * 45)
		{
			_pos1.x -= BLOCK_SIZE * 75;
		}
		if (_pos2.x > BLOCK_SIZE * 45)
		{
			_pos2.x -= BLOCK_SIZE * 75;
		}

		_sector1->GetNode()->setPosition(VectorToOgre(_pos1));
		_sector2->GetNode()->setPosition(VectorToOgre(_pos2));
	}

	void Fence::SetVisible(bool visible)
	{
		_sector1->GetNode()->setVisible(visible);
		_sector2->GetNode()->setVisible(visible);
	}

}