#include "BlasterBurst.h"
#include "Render/Drawables/ModelDrawable.h"
#include "Render/Drawables/RectDrawable.h"
#include "Render/ParticleSystem.h"

using namespace std;

namespace CoreEngine
{
	BlasterBurst::BlasterBurst(Vector3 offset, std::string material, float speed)
		: SpaceObject(offset, speed)
	{
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		auto sceneNode = sceneManager->createSceneNode();
		sceneManager->getRootSceneNode()->addChild(sceneNode);
		_sector = new SceneSector(sceneNode);
		sceneNode->setPosition(VectorToOgre(offset));

		std::vector<Vector3> pointList;
		pointList.push_back(Vector3(-1.8f, 0, -0.05f));
		pointList.push_back(Vector3(0.0f, 0, -0.05f));
		pointList.push_back(Vector3(0.0f, 0,  0.05f));
		pointList.push_back(Vector3(-1.8f, 0, 0.05f));
		_rect = make_unique<RectDrawable>(_sector, material, pointList);

		auto sceneNodeChild = sceneManager->createSceneNode();
		sceneNodeChild->setDirection(0, 1, 0);

		sceneNodeChild->setInheritScale(false);
		sceneNodeChild->setScale(0.2f, 0.2f, 0.2f);
		sceneNodeChild->setPosition(0,0,0);
		sceneNode->addChild(sceneNodeChild);

		_glowEffect = new ParticleSystem(sceneNodeChild, "GlowBlaster_%d", "BlasterGlowEffect", 200, 0, true);
	}

	BlasterBurst::~BlasterBurst()
	{
		_rect.reset();
		delete _glowEffect;
		delete _sector;
	}

	void BlasterBurst::Update(float time, float roadSpeed)
	{
		SpaceObject::Update(time, roadSpeed);
		_sector->GetNode()->setPosition(VectorToOgre(_pos));
	}

	bool BlasterBurst::IsDone()
	{
		return _pos.x > BLOCK_SIZE * 5 || _pos.x < -10 * BLOCK_SIZE || _destroyed;
	}

	void BlasterBurst::SetVisible(bool visible)
	{
		_sector->GetNode()->setVisible(visible);
	}

	const Vector3& BlasterBurst::getHalfSize()
	{
		static Vector3 standardBurstSize(1, 0.05f, 0.05f);
		return standardBurstSize;
	}

}