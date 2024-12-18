#pragma once
#include "Behaviour.h"

namespace GameEngine
{
	class COREMODULE_API MonoBehaviour : public Behaviour
	{
	public:
		explicit MonoBehaviour(GameObject* _owner)
			: Behaviour(_owner) {}

		~MonoBehaviour() override = default;

		virtual void Awake() {}
		virtual void OnEnable() {}
		virtual void Start() {}
		virtual void Fixed_Update() {}
		virtual void Update() {}
		virtual void Late_Update() {}
		virtual void OnDestroy() {}
		virtual void OnDisable() {}

		//virtual void OnCollisionEnter(Collision _other) {}
		//virtual void OnCollisionStay(Collision _other) {}
		//virtual void OnCollisionExit(Collision _other) {}

		void Set_Enable(const bool _enabled) final
		{
			if (m_bEnabled != _enabled)
			{
				if (_enabled) OnEnable();
				else OnDisable();
				m_bEnabled = _enabled;
			}
		}

		void Destroy() override = 0;
	};
}


