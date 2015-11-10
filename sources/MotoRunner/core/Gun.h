#pragma once
#include "Basic.h"
#include <vector>
class ModelDrawable;
class SceneSector;

namespace Ogre
{
	class BillboardChain;
}

namespace CoreEngine
{
	class Gun
	{
		Vector3 _pos;

		ModelDrawable* _model;
		SceneSector * _sector;

	public:
		Gun(Vector3 pos);
		virtual ~Gun();

		void SetScale(Vector3 scale);
		void SetPosition(Vector3 position);
		Vector3 GetPosition();
		void LookAt(Vector3 target);
		void Shoot();
		void Update(float time);

		void SetVisible(bool visible);
	};
}