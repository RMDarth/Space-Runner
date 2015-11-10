#include "Basic.h"

namespace CoreEngine
{
	const float Vector3::EPS = 0.001f;

	Vector3::Vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vector3::Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void Vector3::Normalize()
	{
		float sqlength = SquareLength();
		if (sqlength == 0) return;
		float mult = 1.0f / sqrtf(sqlength);
		x = x * mult;
		y = y * mult;
		z = z * mult;
	}

	Vector3i::Vector3i()
	{
		x = 0; y = 0; z = 0;
	}

	Vector3i::Vector3i(int x, int y, int z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
}