#include "CircleDrawable.h"
using namespace std;


CircleDrawable::CircleDrawable(SceneSector * sector, Vector3 pos, float radius, int accuracy)
{
	_sceneSector = sector;

	_manualObject =  RenderProcessor::Instance()->GetSceneManager()->createManualObject(); 

	_manualObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
 
    unsigned point_index = 0;
    for(float theta = 0; theta <= 2 * Ogre::Math::PI; theta += Ogre::Math::PI / accuracy) {
        _manualObject->position(pos.x + radius * cos(theta), pos.y + radius * sin(theta), pos.z);
        _manualObject->index(point_index++);
    }
    _manualObject->index(0); // Rejoins the last point to the first.
    _manualObject->end();

	sector->GetNode()->attachObject(_manualObject);
}

CircleDrawable::~CircleDrawable()
{
	_sceneSector->GetNode()->detachObject(_manualObject);
}

void CircleDrawable::SetMaterial(string name)
{
	_manualObject->setMaterialName(0, name);
}

void CircleDrawable::Update(float deltaTime)
{

}

void CircleDrawable::SetVisible(bool value)
{
	_manualObject->setVisible(value);
}