#include "Renderer.h"

#include "RenderManager.h"

D3DXMATRIX GameEngine::Renderer::Make_BillboardMatrix(const D3DXMATRIX& _worldMat, const D3DXMATRIX& _viewMat)
{
	// 1) 월드 행렬 분해
	D3DXVECTOR3 scale, trans;
	D3DXQUATERNION rot;
	D3DXMatrixDecompose(&scale, &rot, &trans, &_worldMat);

	// 2) 카메라의 Right/Up/Look 벡터
	D3DXVECTOR3 camRight(_viewMat._11, _viewMat._21, _viewMat._31);
	D3DXVECTOR3 camUp(_viewMat._12, _viewMat._22, _viewMat._32);
	D3DXVECTOR3 camLook(_viewMat._13, _viewMat._23, _viewMat._33);

	// 3) 빌보딩용 회전 행렬 구성
	//    (카메라가 보는 방향대로 X=Right, Y=Up, Z=Look 세 축)
	D3DXMATRIX matBillRot(
		camRight.x, camRight.y, camRight.z, 0.f,
		camUp.x, camUp.y, camUp.z, 0.f,
		camLook.x, camLook.y, camLook.z, 0.f,
		0.f, 0.f, 0.f, 1.f
	);

	// 4) 스케일, 회전, 위치 행렬 재합성
	D3DXMATRIX matS, matT;
	D3DXMatrixScaling(&matS, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&matT, trans.x, trans.y, trans.z);

	// 최종: S * (카메라회전) * T
	return matS * matBillRot * matT;
}

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
