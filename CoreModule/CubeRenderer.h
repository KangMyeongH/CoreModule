#pragma once
#include "Renderer.h"

namespace GameEngine
{
	class CubeRenderer : public Renderer
	{
	public:
		//======================================//
		//				constructor				//
		//======================================//
		CubeRenderer() : Renderer(nullptr),
			m_VertexBuffer(nullptr),
			m_IndexBuffer(nullptr),
			m_VertexSize(0),
			m_VertexCnt(0),
			m_TriangleCnt(0)
		{}
		explicit CubeRenderer(GameObject* _owner) : Renderer(_owner),
			m_VertexBuffer(nullptr),
			m_IndexBuffer(nullptr),
			m_VertexSize(0),
			m_VertexCnt(0),
			m_TriangleCnt(0)
		{}
		CubeRenderer(const CubeRenderer& _rhs)
			: Renderer(_rhs),
			m_VertexBuffer(_rhs.m_VertexBuffer),
			m_IndexBuffer(_rhs.m_IndexBuffer),
			m_VertexSize(_rhs.m_VertexSize),
			m_VertexCnt(_rhs.m_VertexCnt),
			m_TriangleCnt(_rhs.m_TriangleCnt)
		{
			m_VertexBuffer->AddRef();
			m_IndexBuffer->AddRef();
		}
		~CubeRenderer() override = default;

		//======================================//
		//				 method					//
		//======================================//

		//vertex buffer 및 index buffer 생성
		void Ready_Buffer(LPDIRECT3DDEVICE9 _device);

		//화면에 출력
		void Render(LPDIRECT3DDEVICE9 _device) override;

		Component* Clone() const override
		{
			return new CubeRenderer(*this);
		}
		void Destroy() override;


		//======================================//
		//				 serialize				//
		//======================================//
		void to_json(nlohmann::ordered_json& _j) override
		{
			std::string type = "CubeRenderer";
			_j = nlohmann::json{
				{"type", type}
			};
		}
		void from_json(const nlohmann::ordered_json& _j) override
		{
			
		}

	private:
		//Cube Vertex 구조체
		struct VTXCUBE
		{
			Vector3 Position;
			Vector3 TextureUV;
		};
		const ULONG FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

		//Cube Index 구조체
		struct INDEX16
		{
			USHORT  _0;
			USHORT  _1;
			USHORT  _2;

		};

		LPDIRECT3DVERTEXBUFFER9		m_VertexBuffer;
		LPDIRECT3DINDEXBUFFER9		m_IndexBuffer;

		UINT m_VertexSize;
		UINT m_VertexCnt;
		UINT m_TriangleCnt;
	};
	REGISTER_COMPONENT(CubeRenderer)
}

