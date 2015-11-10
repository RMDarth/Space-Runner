#include "LineDrawable.h"
using namespace std;

Ogre::MaterialPtr LineDrawable::_material;

LineDrawable::LineDrawable(SceneSector * sector, vector<Vector3> posList)
{
	_sceneSector = sector;

	_manualObject =  RenderProcessor::Instance()->GetSceneManager()->createManualObject(); 

	if (_material.isNull())
	{
		_material = Ogre::MaterialManager::getSingleton().create("LineMaterial", "General"); 
		_material->setReceiveShadows(false); 
		_material->getTechnique(0)->setLightingEnabled(true); 
		_material->getTechnique(0)->getPass(0)->setDiffuse(0,0,1,0); 
		_material->getTechnique(0)->getPass(0)->setAmbient(0,0,1); 
		_material->getTechnique(0)->getPass(0)->setSelfIllumination(0,0,1); 
	}
 
 
	_manualObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST); 
	for (auto i = posList.begin(); i != posList.end(); i++)
	{
		_manualObject->position(VectorToOgre(*i));
	}

	_manualObject->end(); 

	sector->GetNode()->attachObject(_manualObject);
}

LineDrawable::~LineDrawable()
{
	_sceneSector->GetNode()->detachObject(_manualObject);
}

void LineDrawable::SetMaterial(string name)
{
	_manualObject->setMaterialName(0, name);
}

void LineDrawable::Update(float deltaTime)
{

}

void LineDrawable::SetVisible(bool value)
{
	_manualObject->setVisible(value);
}