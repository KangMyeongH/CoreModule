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
