#include "ParticleSystem.h"

int ParticleSystem::_count = 0;

ParticleSystem::ParticleSystem(Ogre::SceneNode* node, std::string nameTemplate, std::string particleTemplate, float time, int priority)
{
	_count++;
	char name[100];
	sprintf(name, nameTemplate.c_str(), _count);
	_system = RenderProcessor::Instance()->GetSceneManager()->createParticleSystem(name, particleTemplate);
	node->attachObject(_system);
	_system->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY + priority);

	_node = node;
	_time = time;
}

void ParticleSystem::Update(float deltaTime)
{
	_time -= deltaTime;
	
	if (_time < 0 && _node != nullptr)
	{
		_node->getParent()->removeChild(_node);
		_node = nullptr;
	}
}

bool ParticleSystem::IsFinished()
{
	return _node == nullptr;
}

void ParticleSystem::FastForward(float time)
{
	_system->fastForward(time);
}