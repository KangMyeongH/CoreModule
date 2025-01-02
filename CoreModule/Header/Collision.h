#pragma once
#include "core_math.h"

namespace GameEngine
{
	class GameObject;
	class Collider;

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
	};
}

