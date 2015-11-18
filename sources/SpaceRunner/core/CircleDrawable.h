#include "IDrawable.h"
#include "PrecompileHeaders.h"
#include "RenderProcessor.h"
#include "SceneSector.h"
#include <vector>

class CircleDrawable : public IDrawable
{
	Ogre::ManualObject * _manualObject;
	SceneSector * _sceneSector;

public:
	CircleDrawable(SceneSector * sector, Vector3 pos, float radius, int accuracy = 35);
	virtual ~CircleDrawable();

	void SetMaterial(std::string material);

	virtual void Update(float deltaTime);
	virtual void SetVisible(bool value);
};

