#include "MonoBehaviourManager.h"

#include "GameObject.h"

IMPLEMENT_SINGLETON(GameEngine::MonoBehaviourManager)

void GameEngine::MonoBehaviourManager::Fixed_Update() const
{
	for (const auto& monoBehaviour : m_MonoBehaviours)
	{
		if (monoBehaviour->Is_Enabled() && monoBehaviour->Get_GameObject()->Is_Active())
		{
			monoBehaviour->Fixed_Update();
		}
	}
}

void GameEngine::MonoBehaviourManager::Update() const
{
	for (const auto& monoBehaviour : m_MonoBehaviours)
	{
		if (monoBehaviour->Is_Enabled() && monoBehaviour->Get_GameObject()->Is_Active())
		{
			monoBehaviour->Update();
		}
	}
}

void GameEngine::MonoBehaviourManager::Late_Update() const
{
	for (const auto& monoBehaviour : m_MonoBehaviours)
	{
		if (monoBehaviour->Is_Enabled() && monoBehaviour->Get_GameObject()->Is_Active())
		{
			monoBehaviour->Late_Update();
		}
	}
}

void GameEngine::MonoBehaviourManager::On_Destroy() const
{
	for (const auto& obj : m_DestroyQueue)
	{
		obj->On_Destroy();
		obj->Get_GameObject()->Remove_Component(obj);
		obj->Set_Owner(nullptr);
	}
}

GameEngine::MonoBehaviourManager::~MonoBehaviourManager()
{
	for (auto& mb : m_MonoBehaviours)
	{
		delete mb;
	}

	for (auto& mb : m_RegisterQueue)
	{
		delete mb;
	}

	for (auto& mb : m_DestroyQueue)
	{
		delete mb;
	}
}

void GameEngine::MonoBehaviourManager::Release()
{
	for (auto& mb : m_MonoBehaviours)
	{
		delete mb;
	}

	for (auto& mb : m_RegisterQueue)
	{
		delete mb;
	}

	for (auto& mb : m_DestroyQueue)
	{
		delete mb;
	}

	m_MonoBehaviours.clear();
	m_RegisterQueue.clear();
	m_DestroyQueue.clear();
}
