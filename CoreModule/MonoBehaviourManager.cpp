#include "MonoBehaviourManager.h"

IMPLEMENT_SINGLETON(GameEngine::MonoBehaviourManager)

GameEngine::MonoBehaviourManager::~MonoBehaviourManager()
{
	for (auto& mb : m_MonoBehaviours)
	{
		delete mb;
	}

	for (auto& mb : m_PendingQueue)
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

	for (auto& mb : m_PendingQueue)
	{
		delete mb;
	}

	for (auto& mb : m_DestroyQueue)
	{
		delete mb;
	}

	m_MonoBehaviours.clear();
	m_PendingQueue.clear();
	m_DestroyQueue.clear();
}
