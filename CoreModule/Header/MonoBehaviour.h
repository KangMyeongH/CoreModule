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
		virtual void On_Enable() {}
		virtual void Start() {}
		virtual void Fixed_Update() {}
		virtual void Update() {}
		virtual void Late_Update() {}
		virtual void On_Destroy() {}
		virtual void On_Disable() {}

		//virtual void OnCollisionEnter(Collision _other) {}
		//virtual void OnCollisionStay(Collision _other) {}
		//virtual void OnCollisionExit(Collision _other) {}

		void Set_Enable(const bool _enabled) final
		{
			if (m_bEnabled != _enabled)
			{
				if (_enabled) On_Enable();
				else On_Disable();
				m_bEnabled = _enabled;
			}
		}

		void Destroy() override;

	public:
		void to_json(nlohmann::ordered_json& _j) override = 0;
		void from_json(const nlohmann::ordered_json& _j) override = 0;
	};
}


