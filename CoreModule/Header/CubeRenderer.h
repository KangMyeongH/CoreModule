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
		{
			m_VertexSize = sizeof(VTXCUBE);
			m_VertexCnt = 8;
			m_TriangleCnt = 12;
			m_FVF = FVF_COLOR;
		}
		explicit CubeRenderer(GameObject* _owner) : Renderer(_owner)
		{
			m_VertexSize = sizeof(VTXCUBE);
			m_VertexCnt = 8;
			m_TriangleCnt = 12;
			m_FVF = FVF_COLOR;
		}
		CubeRenderer(const CubeRenderer& _rhs) : Renderer(_rhs)
		{
			m_VertexSize = sizeof(VTXCUBE);
			m_VertexCnt = 8;
			m_TriangleCnt = 12;
			m_FVF = FVF_COLOR;
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

