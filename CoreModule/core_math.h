#pragma once

namespace GameEngine
{
	struct Vector2
	{
		
	};
	struct Vector3
	{
	public:
		//======================================//
		//				constructor				//
		//======================================//
		explicit Vector3() : x(0.f), y(0.f), z(0.f) {}
		explicit Vector3(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z) {}
		~Vector3() = default;
		Vector3(const Vector3&) = default;
		Vector3(Vector3&&) = default;
		Vector3& operator=(const Vector3&) = default;
		Vector3& operator=(Vector3&&) = default;

		//======================================//
		//				operators				//
		//======================================//
		Vector3 operator+(const Vector3& rhs) const
		{
			return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
		}
		Vector3& operator+=(const Vector3& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}
		Vector3 operator-(const Vector3& rhs) const
		{
			return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
		}
		Vector3& operator-=(const Vector3& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}
		Vector3 operator*(const float rhs) const
		{
			return Vector3(x * rhs, y * rhs, z * rhs);
		}
		Vector3& operator*=(const float rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			return *this;
		}
		Vector3 operator/(const float rhs) const
		{
			return Vector3(x / rhs, y / rhs, z / rhs);
		}
		Vector3& operator/=(const float rhs)
		{
			x /= rhs;
			y /= rhs;
			z /= rhs;
			return *this;
		}

		Vector3 Normalized() const
		{
			float le
		}

	public:
		float x, y, z;
	};
}
