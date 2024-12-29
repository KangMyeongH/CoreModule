#pragma once
#include "Renderer.h"

namespace GameEngine
{
	class COREMODULE_API CubeRenderer : public Renderer
	{
	public:
		//======================================//
		//				constructor				//
		//======================================//
		CubeRenderer() : Renderer(nullptr)
		{}
		explicit CubeRenderer(GameObject* _owner) : Renderer(_owner)
		{}
		CubeRenderer(const CubeRenderer& _rhs) : Renderer(_rhs)
		{
		}
		~CubeRenderer() override = default;

		//======================================//
		//				 method					//
		//======================================//

	public:
		//vertex buffer 및 index buffer 생성
		void Ready_Buffer(LPDIRECT3DDEVICE9 _device) override;

		//화면에 출력
		void Render(LPDIRECT3DDEVICE9 _device) override;

		Component* Clone() const override
		{
			return new CubeRenderer(*this);
		}


		//======================================//
		//				 serialize				//
		//======================================//
		void to_json(nlohmann::ordered_json& _j) override
		{
			std::string type = "CubeRenderer";
			_j = nlohmann::ordered_json{
				{"type", type}
			};
		}
		void from_json(const nlohmann::ordered_json& _j) override
		{
			
		}

	private:

	};
	REGISTER_COMPONENT(CubeRenderer)
}

