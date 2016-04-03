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
		bool _destroyed;

	public:
		SpaceObject(Vector3 offset, float speed);
		virtual ~SpaceObject();

		virtual void Update(float time, float roadSpeed);

		virtual bool IsIntersected(float turn);
		virtual bool IsIntersected(SpaceObject * other);

		virtual bool IsDone();
		virtual void Destroy();
		virtual void SetVisible(bool visible) = 0;

		virtual const Vector3& getPos();

	protected:
		virtual const Vector3& getHalfSize();

	};
}