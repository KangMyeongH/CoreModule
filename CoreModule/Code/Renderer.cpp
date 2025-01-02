#include "Renderer.h"

#include "RenderManager.h"

void GameEngine::Renderer::Destroy()
{
	RenderManager::GetInstance().Remove_Renderer(this);
}

void GameEngine::Renderer::Render_Buffer(LPDIRECT3DDEVICE9 _device) const
{
	//Material 적용
	_device->SetMaterial(&m_Material);

	//Buffer 출력
	_device->SetStreamSource(0, m_VertexBuffer, 0, m_VertexSize);
	_device->SetFVF(m_FVF);

	_device->SetIndices(m_IndexBuffer);
	_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_VertexCnt, 0, m_TriangleCnt);
}

void GameEngine::Renderer::Get_Buffer(LPDIRECT3DVERTEXBUFFER9& _vertexBuffer, LPDIRECT3DINDEXBUFFER9& _indexBuffer)
{
	_vertexBuffer = m_VertexBuffer;
	_indexBuffer = m_IndexBuffer;
}

void GameEngine::Renderer::Set_Buffer(LPDIRECT3DVERTEXBUFFER9 _vertexBuffer, LPDIRECT3DINDEXBUFFER9 _indexBuffer)
{
	m_VertexBuffer = _vertexBuffer;
	m_IndexBuffer = _indexBuffer;

	m_VertexBuffer->AddRef();
	m_IndexBuffer->AddRef();
}
