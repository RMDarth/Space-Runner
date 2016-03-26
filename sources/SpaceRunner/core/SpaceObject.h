#pragma once
#include "Basic.h"
#include <string>

namespace CoreEngine
{
	class SpaceObject
	{
	protected:
		float _speed;
		Vector3 _pos;

	public:
		SpaceObject(Vector3 offset, float speed);
		virtual ~SpaceObject();

		virtual void Update(float time, float roadSpeed);

		virtual bool IsIntersected(float turn);
		virtual bool IsIntersected(SpaceObject * other);

		virtual bool IsDone();
		virtual void SetVisible(bool visible) = 0;

	protected:
		virtual const Vector3& getHalfSize();

	};
}