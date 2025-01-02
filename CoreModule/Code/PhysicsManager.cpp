#include "PhysicsManager.h"

#include "GameObject.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Scene.h"

IMPLEMENT_SINGLETON(GameEngine::PhysicsManager)

GameEngine::PhysicsManager::~PhysicsManager()
{
	Release();
}

void GameEngine::PhysicsManager::Physics_Update(const float _deltaTime) const
{
	for (auto& rigidbody : m_Rigidbodies)
	{
		if (rigidbody->Get_GameObject()->Is_Active())
		{
			rigidbody->Rigidbody_Update(_deltaTime);
		}
	}
}

void GameEngine::PhysicsManager::Add_Rigidbody(Rigidbody* _rigidbody)
{
	m_RegisterRigidbodyQueue.push_back(_rigidbody);
}

void GameEngine::PhysicsManager::Remove_Rigidbody(Rigidbody* _rigidbody)
{
	for (auto& rb : m_DestroyRigidbodyQueue)
	{
		if (rb == _rigidbody)
		{
			return;
		}
	}

	m_DestroyRigidbodyQueue.push_back(_rigidbody);
}

void GameEngine::PhysicsManager::Register_Rigidbody()
{
	for (auto it = m_RegisterRigidbodyQueue.begin(); it != m_RegisterRigidbodyQueue.end();)
	{
		m_Rigidbodies.push_back(*it);
		it = m_RegisterRigidbodyQueue.erase(it);
	}

	m_RegisterRigidbodyQueue.clear();
}

void GameEngine::PhysicsManager::Destroy_Rigidbody()
{
	for (auto& rb : m_DestroyRigidbodyQueue)
	{
		GameObjectList* objList = Scene::GetInstance().Get_GameObjectList();
		for (auto& gameObject : *objList)
		{
			if (gameObject == rb->Get_GameObject())
			{
				gameObject->Remove_Component(rb);
				break;
			}
		}

		delete rb;
		m_Rigidbodies.erase(std::remove(m_Rigidbodies.begin(), m_Rigidbodies.end(), rb), m_Rigidbodies.end());
	}

	m_DestroyRigidbodyQueue.clear();
}

void GameEngine::PhysicsManager::Release()
{
	for (const auto& rb : m_Rigidbodies)
	{
		delete rb;
	}

	for (const auto& rb : m_RegisterRigidbodyQueue)
	{
		delete rb;
	}

	m_Rigidbodies.clear();
	m_RegisterRigidbodyQueue.clear();
	m_DestroyRigidbodyQueue.clear();
}
