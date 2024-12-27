#pragma once
#include <list>
#include <vector>

#include "MonoBehaviour.h"

namespace GameEngine
{
	class GameObject;

	class COREMODULE_API MonoBehaviourManager
	{
	private:
		MonoBehaviourManager() = default;
		~MonoBehaviourManager();
	public:
		DECLARE_SINGLETON(MonoBehaviourManager)

	public:
		void Fixed_Update() const;

		void Update() const;

		void Late_Update() const;

		void On_Destroy() const;

		void Add_MonoBehaviour(MonoBehaviour* _monoBehaviour)
		{
			m_RegisterQueue.push_back(_monoBehaviour);
			_monoBehaviour->Awake();
		}

		void Register_MonoBehaviour()
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

		void Destroy_MonoBehaviour()
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
