#pragma once
#include "Basic.h"
#include "SpaceDefs.h"
#include "SpaceObject.h"
#include <string>

class SceneSector;
class ModelDrawable;
class RectDrawable;

namespace Ogre
{
	class Material;
}

namespace CoreEngine
{

	class Barrier : public SpaceObject
	{
		SceneSector * _sector;
		ModelDrawable * _modelList[4];
		RectDrawable * _rectList[2];

		Ogre::Material* _material;
		float _alpha;

		PtrList<SceneSector> _sectorList;

	public:
		Barrier(Vector3 offset);
		~Barrier();
		void Update(float time, float roadSpeed) override;

		void SetVisible(bool visible) override;

	private:
		const Vector3& getHalfSize() override;

	};
}