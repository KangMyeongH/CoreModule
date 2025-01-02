#include "GameObject.h"

GameEngine::GameObject::GameObject(): Object("GameObject"), m_Transform(this), m_bActiveSelf(true), m_bStatic(false)
{
}

GameEngine::GameObject::~GameObject()
{
}

void GameEngine::GameObject::Destroy()
{
	if (!m_bDestroyed)
	{
		m_bDestroyed = true;

		for (auto& comVec : m_ComponentMap)
		{
			for (auto& component : comVec.second)
			{
				component->Destroy();
			}
		}
	}
}

void GameEngine::GameObject::On_CollisionEnter(Collision _other)
{
	for (auto& componentVec : m_ComponentMap)
	{
		for (auto& component : componentVec.second)
		{
			MonoBehaviour* monoBehaviour = dynamic_cast<MonoBehaviour*>(component);

			if (monoBehaviour != nullptr)
			{
				monoBehaviour->On_CollisionEnter(_other);
			}
		}
	}
}

void GameEngine::GameObject::On_CollisionStay(Collision _other)
{
	for (auto& componentVec : m_ComponentMap)
	{
		for (auto& component : componentVec.second)
		{
			MonoBehaviour* monoBehaviour = dynamic_cast<MonoBehaviour*>(component);

			if (monoBehaviour != nullptr)
			{
				monoBehaviour->On_CollisionStay(_other);
			}
		}
	}
}

void GameEngine::GameObject::On_CollisionExit(Collision _other)
{
	for (auto& componentVec : m_ComponentMap)
	{
		for (auto& component : componentVec.second)
		{
			MonoBehaviour* monoBehaviour = dynamic_cast<MonoBehaviour*>(component);

			if (monoBehaviour != nullptr)
			{
				monoBehaviour->On_CollisionExit(_other);
			}
		}
	}
}
