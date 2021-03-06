#include "SceneSector.h"


SceneSector::SceneSector(Ogre::SceneNode* node)
{
	_node = node;
}

void SceneSector::SetScale(Vector3 scale)
{
	_node->setScale(VectorToOgre(scale));
}

Ogre::SceneNode* SceneSector::GetNode()
{
	return _node;
}

SceneSector::~SceneSector()
{
	//_node->getParentSceneNode()->removeChild(_node);
	RenderProcessor::Instance()->GetSceneManager()->destroySceneNode(_node);
}


