#pragma once
#include <cmath>
#include <cstdlib>
#include <string>

#ifdef ANDROID
inline int stoi(std::string str)
{
	return atoi(str.c_str());
}
#endif


namespace CoreEngine
{
	struct Vector3
	{
		static const float EPS;

		float x, y, z;

		Vector3();
		Vector3(float x, float y, float z);

		inline Vector3 operator* (float k) const
		{
    		return Vector3( x * k,  y * k,  z * k);
		}

		inline float operator* (const Vector3& V2) const
		{
			return x * V2.x + y * V2.y + z* V2.z;
		}

		inline Vector3 operator+ (const Vector3& V2) const
		{
			return Vector3 (x + V2.x, y + V2.y, z + V2.z);
		}

		inline Vector3 operator- (const Vector3& V2) const
		{
			return Vector3 (x - V2.x, y - V2.y, z - V2.z);
		}

		inline Vector3 operator-() const
		{
			return Vector3(-x, -y, -z);
		}

		inline bool operator==(const Vector3 V2) const
		{
			return (x - V2.x) < EPS && (y - V2.y) < EPS && (z - V2.z) < EPS;
		}

		inline float SquareLength()
		{
			return x*x + y*y + z*z;
		}

		inline static Vector3 CrossProduct(Vector3 a, Vector3 b)
		{
			return Vector3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
		}

		inline void Abs()
		{
			x = abs(x);
			y = abs(y);
			z = abs(z);
		}

		void Normalize();
	};

	struct Vector3i
	{
		int x, y, z;

		Vector3i();
		Vector3i(int x, int y, int z);

		inline bool operator==(const Vector3i V2) const
		{
			return x == V2.x && y == V2.y && z == V2.z;
		}
	};
}
