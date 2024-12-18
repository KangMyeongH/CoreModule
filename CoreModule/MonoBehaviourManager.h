#pragma once
#include <list>
#include <vector>

#include "GameObject.h"
#include "MonoBehaviour.h"

namespace GameEngine
{
	class COREMODULE_API MonoBehaviourManager
	{
	private:
		MonoBehaviourManager() = default;
		~MonoBehaviourManager();
	public:
		DECLARE_SINGLETON(MonoBehaviourManager)

	public:
		void Fixed_Update()
		{
			for (const auto& monoBehaviour : m_MonoBehaviours)
			{
				if (monoBehaviour->Is_Enabled())
				{
					monoBehaviour->Fixed_Update();
				}
			}
		}

		void Update()
		{
			for (const auto& monoBehaviour : m_MonoBehaviours)
			{
				if (monoBehaviour->Is_Enabled())
				{
					monoBehaviour->Fixed_Update();
				}
			}
		}

		void Late_Update()
		{
			for(const auto& monoBehaviour : m_MonoBehaviours)
			{
				if (monoBehaviour->Is_Enabled())
				{
					monoBehaviour->Late_Update();
				}
			}
		}

		void OnDestroy()
		{
			for (const auto& obj : m_DestroyQueue)
			{
				obj->OnDestroy();
				obj->Get_GameObject()->Remove_Component(obj);
				obj->Set_Owner(nullptr);
			}
		}

		void RegisterForUpdates()
		{
			for (auto it = m_PendingQueue.begin(); it != m_PendingQueue.end();)
			{
				MonoBehaviour* monoBehaviour = *it;

				if (monoBehaviour->Is_Enabled())
				{
					monoBehaviour->OnEnable();
					monoBehaviour->Start();
					m_MonoBehaviours.push_back(monoBehaviour);

					it = m_PendingQueue.erase(it);
				}

				else ++it;
			}
		}

		void ClearDestroyQueue()
		{
			for (auto& obj : m_DestroyQueue)
			{
				delete obj;
				m_MonoBehaviours.erase(std::remove(m_MonoBehaviours.begin(), m_MonoBehaviours.end(), obj), m_MonoBehaviours.end());
			}
			m_DestroyQueue.clear();
		}

	private:
		std::vector<MonoBehaviour*>		m_MonoBehaviours;
		std::list<MonoBehaviour*>		m_PendingQueue;
		std::list<MonoBehaviour*>		m_DestroyQueue;
	};
}
