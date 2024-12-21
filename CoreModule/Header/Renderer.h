#pragma once
#include "Behaviour.h"
namespace GameEngine
{
	class COREMODULE_API Renderer : public Behaviour
	{
	public:
		explicit Renderer(GameObject* _owner) : Behaviour(_owner) {}

		virtual void Render(LPDIRECT3DDEVICE9 _device) = 0;
		Component* Clone() const override = 0;
		void Destroy() override = 0;
		void to_json(nlohmann::ordered_json& _j) override = 0;
		void from_json(const nlohmann::ordered_json& _j) override = 0;
	};

}
