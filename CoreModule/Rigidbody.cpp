#include "Rigidbody.h"

#include "GameObject.h"
#include "PhysicsManager.h"

void GameEngine::Rigidbody::Add_Force(const Vector3& _force)
{
	if (m_Mass != 0.f)
	{
		m_Velocity += _force / m_Mass;
	}

	else
	{
		m_Velocity += _force;
	}
}

void GameEngine::Rigidbody::Rigidbody_Update(const float& _deltaTime)
{
	if (m_UseGravity)
	{
		m_Velocity.y += 9.81f * _deltaTime;
	}

	m_Velocity.x *= 1.0f / (1.0f + m_Drag * _deltaTime);
	m_Velocity.y *= 1.0f / (1.0f + m_Drag * _deltaTime);
	m_Velocity.z *= 1.0f / (1.0f + m_Drag * _deltaTime);

	Transform* transform = &m_Owner->Get_Transform();
	
	if (transform)
	{
		transform->Translate(m_Velocity);
	}
}

void GameEngine::Rigidbody::Destroy()
{
	PhysicsManager::GetInstance().Remove_Rigidbody(this);
}
