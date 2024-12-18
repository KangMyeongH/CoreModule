#pragma once
#include <algorithm>
#include <d3dx9math.h>

#include "cmath"
#include "json.hpp"

namespace GameEngine
{
	template <typename T>
	T clamp(const T& value, const T& min, const T& max)
	{
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	struct Vector2
	{
		// Vector2 ����...? �Ⱦ��Ű�����...��������ͤäӤ������ͤäӤ������֤� �ϴ� API�� ���� ���� ���� ��������
		// TODO : ���Ŀ� ���� ������ ���� �� ����
	public:
		//======================================//
		//				constructor				//
		//======================================//
		explicit Vector2() : x(0.f), y(0.f) {}
		explicit Vector2(const float _x, const float _y) : x(_x), y(_y) {}
		~Vector2() = default;
		Vector2(const Vector2&) = default;
		Vector2(Vector2&&) = default;
		Vector2& operator=(const Vector2&) = default;
		Vector2& operator=(Vector2&&) = default;

		//======================================//
		//				operators				//
		//======================================//
		Vector2 	operator+(const Vector2& rhs) const
		{
			return Vector2{ x + rhs.x, y + rhs.y };
		}
		Vector2& 	operator+=(const Vector2& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}
		Vector2 	operator-(const Vector2& rhs) const
		{
			return Vector2{ x - rhs.x, y - rhs.y };
		}
		Vector2& 	operator-=(const Vector2& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}
		Vector2 	operator*(const float rhs) const
		{
			return Vector2{ x * rhs, y * rhs };
		}
		Vector2& 	operator*=(const float rhs)
		{
			x *= rhs;
			y *= rhs;
			return *this;
		}
		Vector2 	operator/(const float rhs) const
		{
			return Vector2{ x / rhs, y / rhs };
		}
		Vector2& 	operator/=(const float rhs)
		{
			x /= rhs;
			y /= rhs;
			return *this;
		}
		bool		operator==(const Vector2& rhs) const
		{
			return x == rhs.x && y == rhs.y;
		}
		bool		operator!=(const Vector2& rhs) const
		{
			return !(*this == rhs);
		}

		//======================================//
		//				  method				//
		//======================================//
		Vector2 		Normalized() const
		{
			float length = Magnitude();
			if (length != 0.0f)
			{
				return *this / length;
			}
			return *this;
		}
		float 			Magnitude() const
		{
			return sqrt(x * x + y * y);
		}
		float 			SqrMagnitude() const
		{
			return x * x + y * y;
		}

		//======================================//
		//			   static method			//
		//======================================//
		static float	Dot(const Vector2& left, const Vector2& right)
		{
			return left.x * right.x + left.y * right.y;
		}
		static float 	Distance(const Vector2 left, const Vector2 right)
		{
			return sqrt((left.x - right.x) * (left.x - right.x) + (left.y - right.y) * (left.y - right.y));
		}
		static Vector2	Lerp(const Vector2& a, const Vector2& b, float t)
		{
			return a + (b - a) * t;
		}
		// A�� B�� ��ǥ�� �޾� A���� B�� �ٶ󺸴� ����(���� �Ǵ� ��)�� ��ȯ
		static float GetAngle(Vector2 A, Vector2 B)
		{
			// A���� B�� ���� ������ ����
			float dx = B.x - A.x;
			float dy = B.y - A.y;

			// atan2�� ����Ͽ� ���� ��� (����)
			float angle = atan2(dy, dx);

			// ������ ��(degree) ������ ��ȯ
			float angleInDegrees = angle * (180.0f / 3.14159265f);

			return angleInDegrees;  // ��(degree) ������ ���� ��ȯ
		}

	public:
		float x, y;
	};

	struct Vector3 : D3DXVECTOR3
	{
	public:
		//======================================//
		//				constructor				//
		//======================================//
		 Vector3() { x = 0.f; y = 0.f; z = 0.f; }
		explicit Vector3(const float _x, const float _y, const float _z) { x = _x; y = _y; z = _z; }

		//======================================//
		//				operators				//
		//======================================//

		Vector3 	operator+(const Vector3& rhs) const
		{
			return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
		}
		Vector3& 	operator+=(const Vector3& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}
		Vector3 	operator-(const Vector3& rhs) const
		{
			return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
		}
		Vector3& 	operator-=(const Vector3& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}
		Vector3 	operator*(const float rhs) const
		{
			return Vector3(x * rhs, y * rhs, z * rhs);
		}
		Vector3& 	operator*=(const float rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			return *this;
		}
		Vector3 	operator/(const float rhs) const
		{
			return Vector3(x / rhs, y / rhs, z / rhs);
		}
		Vector3& 	operator/=(const float rhs)
		{
			x /= rhs;
			y /= rhs;
			z /= rhs;
			return *this;
		}
		bool 		operator==(const Vector3& rhs) const
		{
			return x == rhs.x && y == rhs.y && z == rhs.z;
		}
		bool 		operator!=(const Vector3& rhs) const
		{
			return !(*this == rhs);
		}

		//======================================//
		//				  method				//
		//======================================//
		// ����ȭ ��Ų ���͸� ��ȯ��
		Vector3 	Normalized() const
		{
			float length = sqrt(x * x + y * y + z * z);
			if (length != 0.0f)
			{
				return *this / length;
			}
			return *this; 
		}

		// ���͸� ����ȭ ��Ŵ
		void 		Normalize()
		{
			const float length = sqrt(x * x + y * y + z * z);
			*this /= length;
		}

		// ������ ũ�⸦ ��ȯ��
		float 		Magnitude() const
		{
			return sqrt(x * x + y * y + z * z);
		}

		// �������� �������� ������ ũ�⸦ ��ȯ��
		float 		SqrMagnitude() const
		{
			return x * x + y * y + z * z;
		}

		//======================================//
		//			   static method			//
		//======================================//
		// ����ȭ�� Ư�� ���� ���͸� ��ȯ
		static Vector3	Forward()
		{
			return Vector3(0.f, 0.f, 1.f);
		}
		static Vector3	Back()
		{
			return Vector3(0.f, 0.f, -1.f);
		}
		static Vector3	Down()
		{
			return Vector3(0.f, -1.f, 0.f);
		}
		static Vector3	Up()
		{
			return Vector3(0.f, 1.f, 0.f);
		}
		static Vector3	Left()
		{
			return Vector3(-1.f, 0.f, 0.f);
		}
		static Vector3	Right()
		{
			return Vector3(1.f, 0.f, 0.f);
		}

		// ����ȭ �� ���͸� ��ȯ��
		static Vector3 	Normalize(const Vector3& value)
		{
			float length = value.Magnitude();
			if (length != 0.0f)
			{
				return value / length;
			}
			return value;
		}

		// �� ������ ������ ��ȯ��
		static float 	Dot(const Vector3& lhs, const Vector3& rhs)
		{
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
		}

		// �� ������ ������ ��ȯ��
		static Vector3 	Cross(const Vector3& lhs, const Vector3& rhs)
		{
			return Vector3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
		}

		// �� ������ �Ÿ��� ����.
		static float 	Distance(const Vector3& startVec, const Vector3& endVec)
		{
			const Vector3 diff = endVec - startVec;

			return diff.Magnitude();
		}

		// �� ������ ���� ���͸� ��ȯ��
		static Vector3 	Direction(const Vector3& startVec, const Vector3& endVec)
		{
			const Vector3 diff = endVec - startVec;

			return diff.Normalized();
		}

		// ���� ����
		static Vector3	Lerp(const Vector3& start, const Vector3& end, float t)
		{
			t = clamp(t, 0.0f, 1.0f);
			return start + (end - start) * t;
		}

		// �� ������ ���� ū ��ҷθ� �̷���� ���͸� ��ȯ
		static Vector3	Max(const Vector3& lhs, const Vector3& rhs)
		{
			return Vector3((std::max)(lhs.x, rhs.x), (std::max)(lhs.y, rhs.y), (std::max)(lhs.z, rhs.z));
		}

		// �� ������ ���� ���� ��ҷθ� �̷���� ���͸� ��ȯ
		static Vector3	Min(const Vector3& lhs, const Vector3& rhs)
		{
			return Vector3((std::min)(lhs.x, rhs.x), (std::min)(lhs.y, rhs.y), (std::min)(lhs.z, rhs.z));
		}

		// TODO : ADD MORE STATIC METHOD!!! (Project(), Angle(), Slerp(), SmoothDamp())

	public:
		// json ��ȯ
		friend void to_json(nlohmann::json& j, const Vector3& v)
		 {
			j = nlohmann::json{ {"x", v.x}, {"y", v.y}, {"z", v.z} };
		 }

		friend void from_json(const nlohmann::json& j, Vector3& v)
		 {
			j.at("x").get_to(v.x);
			j.at("y").get_to(v.y);
			j.at("z").get_to(v.z);
		 }
	};

	struct AnimationCurve
	{
		#define PI 3.141592f

		// Easing �Լ��� �ð� �帧�� ���� �Ű������� ��ȭ���� �����մϴ�.
		// ��κ��� ���� �繰���� ������ �ӵ��� �̵����� �ʰ�, ��� �����ϰų� ��� �������� �ʽ��ϴ�.
		// ������ ���� ���ڸ�, ó������ ������ ���ٰ� ���� �� ������ ���뿡�� õõ�� ���ϴ�.
		// �繰�� �ٴڿ� ����Ʈ���� ���� �繰�� �Ʒ��� �����ϴ� �繰�� �ٴ��� ���� �� ƨ�� �ö�ɴϴ�.
		//
		// 1. easeIn �迭 �Լ����� ���� ���ӵǴ� ȿ���� �����մϴ�.
		// 2. easeOut �迭 �Լ����� �ݴ�� ���� ���ӵǴ� ȿ���� �����մϴ�.
		// 3. easeInOut �迭 �Լ����� ���Ӱ� ������ ��� ����Ǿ� ���ʿ��� �ε巴�� ��ȯ�˴ϴ�.
		// �� �Լ����� t�� 0���� 1���� ����Ǹ�, ������ ��� �����մϴ�. �ʿ信 �°� ����Ͻø� �˴ϴ�.
		//	���� ��ũ : https://easings.net/ko

		// ����
		// a�� b���� �����̴µ� �ɸ��� �ð�(duration)�� �ִٰ��մϴ�.
		// POINT StartPosition = a; // ������ ��ǥ
		// POINT EndPosition   = b; // ������ ��ǥ
		//
		// float mCurrentTime = 0.f;
		// float mDurationTime = 2.f; // a�� b���� �����̴µ� 2�ʰ� �ɸ���.
		// void MoveAtoB()
		//{
		// 	 mCurrentTime += mDeltaTime;             // �����Ӵ� �ɸ��� �ð� �����ֱ�
		//   float t = mCurrentTime / mDurationTime;
		//   
		//   a = Lerp(StartPosition, EndPosition, EaseInSine(t));
		//}

		template<typename T>
		static T 		Lerp(T a, T b, float t)
		{
			return a + t * (b - a);
		}

		static float 	EaseInSine(float t)
		{
			return 1 - std::cos((t * PI) / 2);
		}
		static float 	EaseOutSine(float t)
		{
			return std::sin((t * PI) / 2);
		}
		static float 	EaseInOutSine(float t)
		{
			return -(std::cos(PI * t) - 1) / 2;
		}
		static float 	EaseInQuad(float t)
		{
			return t * t;
		}
		static float 	EaseOutQuad(float t)
		{
			return 1 - (1 - t) * (1 - t);
		}
		static float 	EaseInOutQuad(float t)
		{
			return t < 0.5f ? 2.f * t * t : 1.f - std::pow(-2.f * t + 2.f, 2.f) * 0.5f;
		}
		static float 	EaseInCubic(float t)
		{
			return t * t * t;
		}
		static float 	EaseOutCubic(float t)
		{
			return 1.f - std::pow(1.f - t, 3.f);
		}
		static float 	EaseInOutCubic(float t)
		{
			return t < 0.5 ? 4.f * t * t * t : 1.f - std::pow(-2.f * t + 2.f, 3.f) * 0.5f;
		}
		static float 	EaseInQuart(float t)
		{
			return t * t * t * t;
		}
		static float 	EaseOutQuart(float t)
		{
			return 1.f - std::pow(1.f - t, 4.f);
		}
		static float 	EaseInOutQuart(float t)
		{
			return t < 0.5 ? 8.f * t * t * t * t : 1.f - std::pow(-2.f * t + 2.f, 4.f) * 0.5f;
		}
		static float 	EaseInQuint(float t)
		{
			return t * t * t * t * t;
		}
		static float 	EaseOutQuint(float t)
		{
			return 1.f - std::pow(1.f - t, 5.f);
		}
		static float 	EaseInOutQuint(float t)
		{
			return t < 0.5 ? 16.f * t * t * t * t * t : 1.f - std::pow(-2.f * t + 2.f, 5.f) * 0.5f;
		}
		static float 	EaseInExpo(float t)
		{
			return t == 0 ? 0 : std::pow(2.f, 10.f * t - 10.f);
		}
		static float 	EaseOutExpo(float t)
		{
			return t == 1.f ? 1.f : 1.f - std::pow(2.f, -10.f * t);
		}
		static float 	EaseInOutExpo(float t)
		{
			if (t == 0) return 0;
			if (t == 1) return 1;
			return t < 0.5 ? std::pow(2.f, 20.f * t - 10.f) / 2.f : (2.f - std::pow(2.f, -20.f * t + 10.f)) * 0.5f;
		}
		static float 	EaseInCirc(float t)
		{
			return 1.f - std::sqrt(1.f - std::pow(t, 2.f));
		}
		static float 	EaseOutCirc(float t)
		{
			return std::sqrt(1.f - std::pow(t - 1.f, 2.f));
		}
		static float 	EaseInOutCirc(float t)
		{
			return t < 0.5f ? (1.f - std::sqrt(1.f - std::pow(2.f * t, 2.f))) / 2.f : (std::sqrt(1.f - std::pow(-2.f * t + 2.f, 2.f)) + 1.f) * 0.5f;
		}
		static float 	EaseInBack(float t)
		{
			const float c1 = 1.70158f;
			const float c3 = c1 + 1.f;
			return c3 * t * t * t - c1 * t * t;
		}
		static float 	EaseOutBack(float t)
		{
			const float c1 = 1.70158f;
			const float c3 = c1 + 1.f;
			return 1.f + c3 * std::pow(t - 1.f, 3.f) + c1 * std::pow(t - 1.f, 2.f);
		}
		static float 	EaseInOutBack(float t)
		{
			const float c1 = 1.70158f;
			const float c2 = c1 * 1.525f;
			return t < 0.5f
				? (std::pow(2.f * t, 2.f) * ((c2 + 1.f) * 2.f * t - c2)) * 0.5f
				: (std::pow(2.f * t - 2.f, 2.f) * ((c2 + 1.f) * (t * 2.f - 2.f) + c2) + 2.f) * 0.5f;
		}
		static float 	EaseInElastic(float t)
		{
			const float c4 = (2.f * PI) / 3.f;
			return t == 0 ? 0 : t == 1.f ? 1.f : -std::pow(2.f, 10.f * t - 10.f) * std::sin((t * 10.f - 10.75f) * c4);
		}
		static float 	EaseOutElastic(float t)
		{
			const float c4 = (2.f * PI) / 3.f;
			return t == 0 ? 0 : t == 1.f ? 1.f : std::pow(2.f, -10.f * t) * std::sin((t * 10.f - 0.75f) * c4) + 1.f;
		}
		static float 	EaseInOutElastic(float t)
		{
			const float c5 = (2 * PI) / 4.5f;
			if (t == 0) return 0;
			if (t == 1.f) return 1.f;
			return t < 0.5f
				? -(std::pow(2.f, 20.f * t - 10.f) * std::sin((20.f * t - 11.125f) * c5)) * 0.5f
				: (std::pow(2.f, -20.f * t + 10.f) * std::sin((20.f * t - 11.125f) * c5)) * 0.5f + 1;
		}
		static float 	EaseInBounce(float t)
		{
			return 1 - EaseOutBounce(1 - t);
		}
		static float 	EaseOutBounce(float t)
		{
			const float n1 = 7.5625f;
			const float d1 = 2.75f;

			if (t < 1 / d1) {
				return n1 * t * t;
			}
			else if (t < 2 / d1) {
				t -= 1.5f / d1;
				return n1 * t * t + 0.75f;
			}
			else if (t < 2.5 / d1) {
				t -= 2.25f / d1;
				return n1 * t * t + 0.9375f;
			}
			else {
				t -= 2.625f / d1;
				return n1 * t * t + 0.984375f;
			}
		}
		static float 	EaseInOutBounce(float t)
		{
			return t < 0.5f
				? (1 - EaseOutBounce(1 - 2 * t)) * 0.5f
				: (1 + EaseOutBounce(2 * t - 1)) * 0.5f;
		}
	};
}
