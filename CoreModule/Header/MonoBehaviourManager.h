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
		void Fixed_Update() const
		{
			for (const auto& monoBehaviour : m_MonoBehaviours)
			{
				if (monoBehaviour->Is_Enabled())
				{
					monoBehaviour->Fixed_Update();
				}
			}
		}

		void Update() const
		{
			for (const auto& monoBehaviour : m_MonoBehaviours)
			{
				if (monoBehaviour->Is_Enabled())
				{
					monoBehaviour->Fixed_Update();
				}
			}
		}

		void Late_Update() const
		{
			for(const auto& monoBehaviour : m_MonoBehaviours)
			{
				if (monoBehaviour->Is_Enabled())
				{
					monoBehaviour->Late_Update();
				}
			}
		}

		void On_Destroy() const
		{
			for (const auto& obj : m_DestroyQueue)
			{
				obj->OnDestroy();
				obj->Get_GameObject()->Remove_Component(obj);
				obj->Set_Owner(nullptr);
			}
		}

		void Add_MonoBehaviour(MonoBehaviour* _monoBehaviour)
		{
			m_RegisterQueue.push_back(_monoBehaviour);
			_monoBehaviour->Awake();
		}

		void RegisterForUpdates()
		{
			for (auto it = m_RegisterQueue.begin(); it != m_RegisterQueue.end();)
			{
				MonoBehaviour* monoBehaviour = *it;

				if (monoBehaviour->Is_Enabled())
				{
					monoBehaviour->OnEnable();
					monoBehaviour->Start();
					m_MonoBehaviours.push_back(monoBehaviour);

					it = m_RegisterQueue.erase(it);
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

		void Release();

	private:
		std::vector<MonoBehaviour*>		m_MonoBehaviours;
		std::list<MonoBehaviour*>		m_RegisterQueue;
		std::list<MonoBehaviour*>		m_DestroyQueue;
	};
}
