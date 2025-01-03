#pragma once
#include "Component.h"

namespace GameEngine
{
	class COREMODULE_API Behaviour : public Component
	{
	public:
		explicit Behaviour(GameObject* _owner) : Component(_owner), m_bEnabled(true) {}
		~Behaviour() override = default;

		virtual void 	Set_Enable(const bool _enabled)
		{
			m_bEnabled = _enabled;
		}
		bool 			Is_Enabled() const { return m_bEnabled; }
		void 			Destroy() override = 0;


	public:
		void to_json(nlohmann::ordered_json& _j) override = 0;
		void from_json(const nlohmann::ordered_json& _j) override = 0;
		Component* Clone() const override = 0;

	protected:
		bool m_bEnabled;
	};
}


