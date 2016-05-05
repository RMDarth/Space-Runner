#pragma once
#include "Basic.h"
#include <string>

class SceneSector;

namespace CoreEngine
{
	class SpaceObject
	{
	protected:
		float _speed;
		Vector3 _pos;
		bool _destroyed;

		SceneSector * _sector;

	public:
		SpaceObject(Vector3 offset, float speed);
		virtual ~SpaceObject();

		virtual void Update(float time, float roadSpeed);

		virtual bool IsIntersected(float turn);
		virtual bool IsIntersected(SpaceObject * other);

		virtual bool IsDone();
		virtual bool Passed();
		virtual void Destroy();
		virtual void SetVisible(bool visible) = 0;

		virtual const Vector3& getPos() const;
		virtual SceneSector* getSector();

	protected:
		virtual const Vector3& getHalfSize();

	};
}