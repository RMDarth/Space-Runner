#include "Fence.h"
#include "SceneSector.h"
#include "RectDrawable.h"

using namespace std;

namespace CoreEngine
{
	Fence::Fence()
	{
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		auto sceneNode1 = sceneManager->createSceneNode();
		sceneManager->getRootSceneNode()->addChild(sceneNode1);
		_sector = make_unique<SceneSector>(sceneNode1);
        _alpha = 0;

		Generate();
	}


	Fence::~Fence()
	{
	}

	void Fence::Generate()
	{
		std::vector<Vector3> pointList;
		pointList.push_back(Vector3(-BLOCK_SIZE * 70.01f, 0.0f, -BLOCK_SIZE * 1.5));
		pointList.push_back(Vector3(-BLOCK_SIZE * 70.01f, 1.0f, -BLOCK_SIZE * 1.5));
		pointList.push_back(Vector3(BLOCK_SIZE * 10.01f, 1.0f, -BLOCK_SIZE * 1.5));
		pointList.push_back(Vector3(BLOCK_SIZE * 10.01f, 0.0f, -BLOCK_SIZE * 1.5));
		_modelLeft = make_unique<RectDrawable>(_sector.get(), "FenceMaterial", pointList);

		pointList.clear();
        pointList.push_back(Vector3(-BLOCK_SIZE * 70.01f, 0.0f, BLOCK_SIZE * 1.5));
        pointList.push_back(Vector3(-BLOCK_SIZE * 70.01f, 1.0f, BLOCK_SIZE * 1.5));
        pointList.push_back(Vector3(BLOCK_SIZE * 10.01f, 1.0f, BLOCK_SIZE * 1.5));
        pointList.push_back(Vector3(BLOCK_SIZE * 10.01f, 0.0f, BLOCK_SIZE * 1.5));
		_modelRight = make_unique<RectDrawable>(_sector.get(), "FenceMaterial", pointList);

        _material = Ogre::MaterialManager::getSingleton().getByName("FenceMaterial").get();
	}

	void Fence::Update(float time, float roadSpeed)
	{
        _alpha -= (time * 0.01 + time * roadSpeed * 0.1);
        while (_alpha < 0.0f)
            _alpha += 1.0f;

        _material->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("color", Ogre::ColourValue(1, 1, 1, _alpha));
	}

	void Fence::SetVisible(bool visible)
	{
		_sector->GetNode()->setVisible(visible);
	}

}