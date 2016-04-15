#include "ModelDrawable.h"
#include "OgreDistanceLodStrategy.h"
#include "OgrePixelCountLodStrategy.h"

using namespace std;

ModelDrawable::ModelDrawable(SceneSector * sector, const std::string& name)
{
	_sceneSector = sector;

	_entity = RenderProcessor::Instance()->GetSceneManager()->createEntity(name);
	if (_entity->hasSkeleton())
	{
		_entity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);
		if (HasLOD())
			_entity->getManualLodLevel(0)->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);
	}
	sector->GetNode()->attachObject(_entity);

	//GenerateLods();
}

void ModelDrawable::SetScale(float scale)
{
	_sceneSector->GetNode()->setScale(scale, scale, scale);
}

void ModelDrawable::SetScale(Vector3 scale)
{
	_sceneSector->GetNode()->setScale(scale.x, scale.y, scale.z);
}


ModelDrawable::~ModelDrawable()
{
	_sceneSector->GetNode()->detachObject(_entity);
}

void ModelDrawable::SetRenderingQueue(int queue)
{
	_entity->setRenderQueueGroup(Ogre::RENDER_QUEUE_MAIN + queue);
}

void ModelDrawable::Update(float deltaTime)
{
	for_each(_stateList.begin(), _stateList.end(), bind(&ModelAnimation::Update, std::placeholders::_1 , deltaTime));

	auto newEnd = remove_if(_stateList.begin(), _stateList.end(), bind(&ModelAnimation::hasEnded, std::placeholders::_1));
	if (newEnd != _stateList.end())
	{
		_stateList.erase(newEnd, _stateList.end());
	}
}

void ModelDrawable::SetVisible(bool value)
{
	_entity->setVisible(value);
}

std::string ModelDrawable::GetMaterialName()
{
	return _entity->getSubEntity(0)->getMaterialName();
}

bool ModelDrawable::HasAnimation()
{
	return _entity->hasSkeleton();
}

bool ModelDrawable::HasLOD()
{
	return _entity->getNumManualLodLevels() > 0;
}

ModelAnimation ModelDrawable::AddAnimation(std::string name)
{
	auto state = _entity->getAnimationState(name);
	Ogre::AnimationState* lodState = nullptr;
	if (_entity->getNumManualLodLevels() > 0)
		lodState = _entity->getManualLodLevel(0)->getAnimationState(name);
	ModelAnimation ma(state, lodState);
	_stateList.push_back(ma);

	return ma;
}

void ModelDrawable::RunAnimation(ModelAnimation anim, bool cycle)
{
	anim.GetState()->setEnabled(true);
	anim.GetState()->setTimePosition(0);
	anim.GetState()->setLoop(cycle);

	if (anim.GetLODState() != nullptr)
	{
		anim.GetLODState()->setEnabled(true);
		anim.GetLODState()->setTimePosition(0);
		anim.GetLODState()->setLoop(cycle);
	}
}

void ModelDrawable::SetAnimationFinish()
{
	for (auto i = _stateList.begin(); i != _stateList.end(); i++)
	{
		i->GetState()->setTimePosition(i->GetState()->getLength());
		i->GetState()->setLoop(false);

		if (i->GetLODState() != nullptr)
		{
			i->GetLODState()->setTimePosition(i->GetLODState()->getLength());
			i->GetLODState()->setLoop(false);
		}
	}
}

void ModelDrawable::StopAnimation(ModelAnimation anim)
{
	anim.GetState()->setEnabled(false);
	if (anim.GetLODState() != nullptr)
		anim.GetLODState()->setEnabled(false);
}

void ModelDrawable::SetMaterial(std::string name, int lod)
{
	if (lod == 0)
		_entity->setMaterialName(name);
	else
		_entity->getManualLodLevel(lod - 1)->setMaterialName(name);
}

void ModelDrawable::GenerateLods()
{

	//pm.generateAutoconfiguredLodLevels(mesh);
}