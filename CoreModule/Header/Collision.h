#pragma once
#include "core_math.h"

namespace GameEngine
{
	class GameObject;
	class Collider;

	struct CollisionData
	{
		CollisionData(): A(nullptr), B(nullptr), Penetration(0)
		{
		}

		CollisionData(Collider* _a, Collider* _b, const Vector3& _normal, const float _penetration)
			: A(_a), B(_b), Normal(_normal), Penetration(_penetration) {}

		// Ãæµ¹ ½Ö
		Collider* A;
		Collider* B;

		// ÃÖ¼Ò °ãÄ§ Ãà
		Vector3 Normal;

		// °ãÄ§·®
		float Penetration;
	};

	class Collision
	{
	public:
		Collision(GameObject* _other, Collider* _collider) :
		m_Other(_other), m_Collider(_collider) {}

		GameObject* 	Get_GameObject() const { return m_Other; }
		Collider* 		Get_Collider() const { return m_Collider; }

	private:
		GameObject* 	m_Other;
		Collider* 		m_Collider;
		Vector3			m_Normal;
		float			m_Penetration;
	};
}

