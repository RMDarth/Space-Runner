#include "RectDrawable.h"
using namespace std;

RectDrawable::RectDrawable(SceneSector * sector, std::string material, vector<Vector3> pointlist)
{
	_sceneSector = sector;
	_manualObject = RenderProcessor::Instance()->GetSceneManager()->createManualObject();

	_manualObject->begin(material, Ogre::RenderOperation::OT_TRIANGLE_LIST);
	_manualObject->position(VectorToOgre(pointlist[0]));
	_manualObject->textureCoord(Ogre::Vector2(0, 0));
	_manualObject->position(VectorToOgre(pointlist[2]));
	_manualObject->textureCoord(Ogre::Vector2(1, 1));
	_manualObject->position(VectorToOgre(pointlist[1]));
	_manualObject->textureCoord(Ogre::Vector2(0, 1));
	_manualObject->position(VectorToOgre(pointlist[3]));
	_manualObject->textureCoord(Ogre::Vector2(1, 0));
	_manualObject->position(VectorToOgre(pointlist[2]));
	_manualObject->textureCoord(Ogre::Vector2(1, 1));
	_manualObject->position(VectorToOgre(pointlist[0]));
	_manualObject->textureCoord(Ogre::Vector2(0, 0));
	_manualObject->end();

	sector->GetNode()->attachObject(_manualObject);
}

RectDrawable::~RectDrawable()
{
	_sceneSector->GetNode()->detachObject(_manualObject);
}

void RectDrawable::SetMaterial(string name)
{
	_manualObject->setMaterialName(0, name);
}

void RectDrawable::Update(float deltaTime)
{
	
}

void RectDrawable::SetVisible(bool value)
{
	_manualObject->setVisible(value);
}