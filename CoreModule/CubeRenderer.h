#pragma once
#include "Renderer.h"

namespace GameEngine
{
	class CubeRenderer : public Renderer
	{
	public:
		CubeRenderer() : Renderer(nullptr) {}
		explicit CubeRenderer(GameObject* _owner) : Renderer(_owner) {}
		CubeRenderer(const CubeRenderer& _rhs) = default;

		Component* Clone() const override;
		void Destroy() override;
		void Render(LPDIRECT3DDEVICE9 _device) override;

		void to_json(nlohmann::ordered_json& _j) override;
		void from_json(const nlohmann::ordered_json& _j) override;
	};
	REGISTER_COMPONENT(CubeRenderer)
}

