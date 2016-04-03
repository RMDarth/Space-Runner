#include "ParticleDrawable.h"
using namespace std;

ParticleDrawable::ParticleDrawable(SceneSector * sector, std::string material, float size)
{
	_sceneSector = sector;
	_manualObject = RenderProcessor::Instance()->GetSceneManager()->createManualObject();
	float halfsize = size * 0.5f;

	_manualObject->begin(material, Ogre::RenderOperation::OT_TRIANGLE_LIST);
	_manualObject->position(Ogre::Vector3(0, -halfsize, -halfsize));
	_manualObject->textureCoord(Ogre::Vector2(0, 0));
	_manualObject->position(Ogre::Vector3(0, halfsize, halfsize));
	_manualObject->textureCoord(Ogre::Vector2(1, 1));
	_manualObject->position(Ogre::Vector3(0, -halfsize, halfsize));
	_manualObject->textureCoord(Ogre::Vector2(0, 1));
	_manualObject->position(Ogre::Vector3(0, halfsize, -halfsize));
	_manualObject->textureCoord(Ogre::Vector2(1, 0));
	_manualObject->position(Ogre::Vector3(0, halfsize, halfsize));
	_manualObject->textureCoord(Ogre::Vector2(1, 1));
	_manualObject->position(Ogre::Vector3(0, -halfsize, -halfsize));
	_manualObject->textureCoord(Ogre::Vector2(0, 0));
	_manualObject->end();

	sector->GetNode()->attachObject(_manualObject);

	_manualObject->setMaterialName(0, material);
}

ParticleDrawable::~ParticleDrawable()
{
	_sceneSector->GetNode()->detachObject(_manualObject);
	delete _sceneSector;
}

void ParticleDrawable::SetMaterial(string name)
{
	_manualObject->setMaterialName(0, name);
}

void ParticleDrawable::Update(float deltaTime)
{

}

void ParticleDrawable::SetVisible(bool value)
{
	_manualObject->setVisible(value);
}