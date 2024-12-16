#pragma once
#include "Component.h"

namespace GameEngine
{
	class Behaviour : public Component
	{
	public:
		explicit Behaviour(GameObject* _owner) : Component(_owner), m_bEnabled(true) {}
		~Behaviour() override = default;

		virtual void 	Set_Enable(const bool _enabled)
		{
			m_bEnabled = _enabled;
		}
		bool 			Is_Enabled() const { return m_bEnabled; }
		Component* 		Clone(GameObject* _newOwner) const override = 0;

	protected:
		bool m_bEnabled;
	};
}

