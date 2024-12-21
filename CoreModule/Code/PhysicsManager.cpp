#include "PhysicsManager.h"

#include "GameObject.h"
#include "Rigidbody.h"
#include "Scene.h"

IMPLEMENT_SINGLETON(GameEngine::PhysicsManager)

GameEngine::PhysicsManager::~PhysicsManager()
{
	Release();
}

void GameEngine::PhysicsManager::Physics_Update(float _deltaTime) const
{

}

void GameEngine::PhysicsManager::Add_Rigidbody(Rigidbody* _rigidbody)
{
	m_RegisterQueue.push_back(_rigidbody);
}

void GameEngine::PhysicsManager::Remove_Rigidbody(Rigidbody* _rigidbody)
{
	for (auto& rb : m_DestroyQueue)
	{
		if (rb == _rigidbody)
		{
			return;
		}
	}

	m_DestroyQueue.push_back(_rigidbody);
}

void GameEngine::PhysicsManager::Register_Rigidbody()
{
	for (auto it = m_RegisterQueue.begin(); it != m_RegisterQueue.end();)
	{
		m_RegisterQueue.push_back(*it);
		it = m_RegisterQueue.erase(it);
	}

	m_RegisterQueue.clear();
}

void GameEngine::PhysicsManager::Destroy_Rigidbody()
{
	for (auto& rb : m_DestroyQueue)
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

	m_DestroyQueue.clear();
}

void GameEngine::PhysicsManager::Release()
{
	for (const auto& rb : m_Rigidbodies)
	{
		delete rb;
	}

	for (const auto& rb : m_RegisterQueue)
	{
		delete rb;
	}

	m_Rigidbodies.clear();
	m_RegisterQueue.clear();
	m_DestroyQueue.clear();
}
