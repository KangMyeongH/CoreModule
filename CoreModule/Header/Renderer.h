#pragma once
#include "Behaviour.h"
namespace GameEngine
{
	class COREMODULE_API Renderer : public Behaviour
	{
	public:
		Renderer() : Behaviour(nullptr),
			m_VertexBuffer(nullptr),
			m_IndexBuffer(nullptr),
			m_VertexSize(0),
			m_VertexCnt(0),
			m_TriangleCnt(0)
		{}
		explicit Renderer(GameObject* _owner) : Behaviour(_owner),
			m_VertexBuffer(nullptr),
			m_IndexBuffer(nullptr),
			m_VertexSize(0),
			m_VertexCnt(0),
			m_TriangleCnt(0)
		{}
		Renderer(const Renderer& _rhs) : Behaviour(_rhs),
			m_VertexBuffer(_rhs.m_VertexBuffer),
			m_IndexBuffer(_rhs.m_IndexBuffer),
			m_VertexSize(_rhs.m_VertexSize),
			m_VertexCnt(_rhs.m_VertexCnt),
			m_TriangleCnt(_rhs.m_TriangleCnt)
		{
			m_VertexBuffer->AddRef();
			m_IndexBuffer->AddRef();
		}
		~Renderer() override
		{
			if (m_VertexBuffer) m_VertexBuffer->Release();
			if (m_IndexBuffer) m_IndexBuffer->Release();
		}

		virtual void Get_Buffer(LPDIRECT3DVERTEXBUFFER9& _vertexBuffer, LPDIRECT3DINDEXBUFFER9& _indexBuffer);
		virtual void Set_Buffer(LPDIRECT3DVERTEXBUFFER9 _vertexBuffer, LPDIRECT3DINDEXBUFFER9 _indexBuffer);

		virtual void Ready_Buffer(LPDIRECT3DDEVICE9 _device) = 0;
		virtual void Render(LPDIRECT3DDEVICE9 _device) = 0;
		Component* Clone() const override = 0;
		void Destroy() override;
		void to_json(nlohmann::ordered_json& _j) override = 0;
		void from_json(const nlohmann::ordered_json& _j) override = 0;

		void Render_Buffer(LPDIRECT3DDEVICE9 _device);

	protected:
		LPDIRECT3DVERTEXBUFFER9		m_VertexBuffer;
		LPDIRECT3DINDEXBUFFER9		m_IndexBuffer;

		UINT m_VertexSize;
		UINT m_VertexCnt;
		UINT m_TriangleCnt;
	};

}
