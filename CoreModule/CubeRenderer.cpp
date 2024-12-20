#include "CubeRenderer.h"

#include "GameObject.h"
#include "Transform.h"

void GameEngine::CubeRenderer::Ready_Buffer(LPDIRECT3DDEVICE9 _device)
{
	m_TriangleCnt = 12;
	m_VertexCnt = 8;
	m_VertexSize = sizeof(VTXCUBE);
	UINT indexSize = sizeof(INDEX16);

	if (E_FAIL == _device->CreateVertexBuffer(m_VertexCnt * m_VertexSize,
		0,
		FVF_CUBE,
		D3DPOOL_MANAGED,
		&m_VertexBuffer,
		NULL))
		return;

	if(E_FAIL == _device->CreateIndexBuffer(m_TriangleCnt * indexSize,
		0,						
		D3DFMT_INDEX16,			
		D3DPOOL_MANAGED,		
		&m_IndexBuffer,			
		NULL))
		return;


	VTXCUBE* pVertex = nullptr;

	m_VertexBuffer->Lock(0, 0, (void**)&pVertex, 0);


	// 전면
	pVertex[0].Position = Vector3{ -1.f, 1.f, -1.f };
	pVertex[0].TextureUV = pVertex[0].Position;

	pVertex[1].Position = Vector3{ 1.f, 1.f, -1.f };
	pVertex[1].TextureUV = pVertex[1].Position;

	pVertex[2].Position = Vector3{ 1.f, -1.f, -1.f };
	pVertex[2].TextureUV = pVertex[2].Position;

	pVertex[3].Position = Vector3{ -1.f, -1.f, -1.f };
	pVertex[3].TextureUV = pVertex[3].Position;

	// 후면
	pVertex[4].Position = Vector3{ -1.f, 1.f, 1.f };
	pVertex[4].TextureUV = pVertex[4].Position;

	pVertex[5].Position = Vector3{ 1.f, 1.f, 1.f };
	pVertex[5].TextureUV = pVertex[5].Position;

	pVertex[6].Position = Vector3{ 1.f, -1.f, 1.f };
	pVertex[6].TextureUV = pVertex[6].Position;

	pVertex[7].Position = Vector3{ -1.f, -1.f, 1.f };
	pVertex[7].TextureUV = pVertex[7].Position;

	m_VertexBuffer->Unlock();

	INDEX16* pIndex = nullptr;

	m_IndexBuffer->Lock(0, 0, (void**)&pIndex, 0);

	// X+
	// 오른쪽 위
	pIndex[0]._0 = 1;
	pIndex[0]._1 = 5;
	pIndex[0]._2 = 6;

	// 왼쪽 아래
	pIndex[1]._0 = 1;
	pIndex[1]._1 = 6;
	pIndex[1]._2 = 2;

	// X-
	// 오른쪽 위
	pIndex[2]._0 = 4;
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 3;

	// 왼쪽 아래
	pIndex[3]._0 = 4;
	pIndex[3]._1 = 3;
	pIndex[3]._2 = 7;

	// y+
	// 오른쪽 위
	pIndex[4]._0 = 4;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 1;

	// 왼쪽 아래
	pIndex[5]._0 = 4;
	pIndex[5]._1 = 1;
	pIndex[5]._2 = 0;

	// y-
	// 오른쪽 위
	pIndex[6]._0 = 3;
	pIndex[6]._1 = 2;
	pIndex[6]._2 = 6;

	// 왼쪽 아래
	pIndex[7]._0 = 3;
	pIndex[7]._1 = 6;
	pIndex[7]._2 = 7;

	// z+
// 오른쪽 위
	pIndex[8]._0 = 7;
	pIndex[8]._1 = 6;
	pIndex[8]._2 = 5;

	// 왼쪽 아래
	pIndex[9]._0 = 7;
	pIndex[9]._1 = 5;
	pIndex[9]._2 = 4;

	// z-
	// 오른쪽 위
	pIndex[10]._0 = 0;
	pIndex[10]._1 = 1;
	pIndex[10]._2 = 2;

	// 왼쪽 아래
	pIndex[11]._0 = 0;
	pIndex[11]._1 = 2;
	pIndex[11]._2 = 3;

	m_IndexBuffer->Unlock();
}

void GameEngine::CubeRenderer::Render(LPDIRECT3DDEVICE9 _device)
{
	//Transfrom 설정
	//render State 설정
	//texture 컴포넌트에서 텍스처 가져와서 Set_Texture()

	_device->SetStreamSource(0, m_VertexBuffer, 0, m_VertexSize);
	_device->SetFVF(FVF_CUBE);

	_device->SetIndices(m_IndexBuffer);
	_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_VertexCnt, 0, m_TriangleCnt);

	//render state 복구
}

void GameEngine::CubeRenderer::Destroy()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();
}

