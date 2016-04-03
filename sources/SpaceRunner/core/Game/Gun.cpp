#include "Gun.h"
#include "Render/Drawables/ModelDrawable.h"
#include "PrecompileHeaders.h"

namespace CoreEngine
{

	Gun::Gun(Vector3 pos)
	{
		_pos = pos;

		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		auto sceneNode = sceneManager->createSceneNode();
		sceneManager->getRootSceneNode()->addChild(sceneNode);
		sceneNode->setPosition(_pos.x, _pos.y, _pos.z);

		_sector = new SceneSector(sceneNode);
		_model = new ModelDrawable(_sector, "lo-cannon.mesh");
	}

	Gun::~Gun()
	{
		_model->SetVisible(false);
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		sceneManager->getRootSceneNode()->removeChild(_sector->GetNode());

		delete _model;
		delete _sector;
	}

	void Gun::SetScale(Vector3 scale)
	{
		_sector->SetScale(scale);
	}

	void Gun::SetPosition(Vector3 position)
	{
		_pos = position;
		_sector->GetNode()->setPosition(VectorToOgre(_pos));
	}

	Vector3 Gun::GetPosition()
	{
		return _pos;
	}



	void Gun::LookAt(Vector3 target)
	{
		//_sector->GetNode()->setOrientation(1, 0, 0, 0);
		_sector->GetNode()->lookAt(VectorToOgre(target), Ogre::SceneNode::TS_WORLD);
		//_sector->GetNode()->setOrientation(VectorToOgre(_pos).getRotationTo(VectorToOgre(target)));
	}

	void Gun::Shoot()
	{
		auto animationToRun = _model->AddAnimation("cannon_shot");
		_model->RunAnimation(animationToRun, false);
	}

	void Gun::Update(float time)
	{
		_model->Update(time);
	}

	void Gun::SetVisible(bool visible)
	{
		_model->SetVisible(visible);
	}
}