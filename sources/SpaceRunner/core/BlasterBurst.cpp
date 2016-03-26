#include "BlasterBurst.h"
#include "ModelDrawable.h"
#include "RectDrawable.h"
#include "SceneSector.h"
#include "RenderProcessor.h"

using namespace std;

namespace CoreEngine
{
	BlasterBurst::BlasterBurst(Vector3 offset, std::string material, int count, float speed, float size)
		: SpaceObject(offset, speed)
	{
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		auto sceneNode = sceneManager->createSceneNode();
		sceneManager->getRootSceneNode()->addChild(sceneNode);
		_sector = new SceneSector(sceneNode);
		sceneNode->setPosition(VectorToOgre(offset));

		std::vector<Vector3> pointList;
		pointList.push_back(Vector3(-1.8f, 0, -0.05));
		pointList.push_back(Vector3(0.0f, 0, -0.05));
		pointList.push_back(Vector3(0.0f, 0,  0.05));
		pointList.push_back(Vector3(-1.8f, 0, 0.05));
		_rect = make_unique<RectDrawable>(_sector, material, pointList);
	}

	BlasterBurst::~BlasterBurst()
	{
		_rect.reset();
		delete _sector;
	}

	void BlasterBurst::Update(float time, float roadSpeed)
	{
		SpaceObject::Update(time, roadSpeed);
		_sector->GetNode()->setPosition(VectorToOgre(_pos));
	}

	bool BlasterBurst::IsDone()
	{
		if (_pos.x > BLOCK_SIZE * 5 || _pos.x < -10 * BLOCK_SIZE)
			return true;
		return false;
	}

	void BlasterBurst::SetVisible(bool visible)
	{
		_sector->GetNode()->setVisible(visible);
	}
}