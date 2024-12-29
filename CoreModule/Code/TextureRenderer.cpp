#include "TextureRenderer.h"

#include "GameObject.h"
#include "Transform.h"

void GameEngine::TextureRenderer::Ready_Buffer(LPDIRECT3DDEVICE9 _device)
{
	m_TriangleCnt = 2;
	m_VertexCnt = 4;
	m_VertexSize = sizeof(VTXTEX);
	UINT indexSize = sizeof(INDEX16);

	if (E_FAIL == _device->CreateVertexBuffer(m_VertexCnt * m_VertexSize,
		0,
		FVF_TEX,
		D3DPOOL_MANAGED,
		&m_VertexBuffer,
		nullptr))
		return;

	if (E_FAIL == _device->CreateIndexBuffer(m_TriangleCnt * indexSize,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_IndexBuffer,
		nullptr))
		return;


	VTXTEX* pVertex = nullptr;
	m_VertexBuffer->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].Position = Vector3{ -1.f, 1.f, 0.f };
	pVertex[0].TextureUV = Vector2{ 0.f, 0.f };

	pVertex[1].Position = Vector3{ 1.f, 1.f, 0.f };
	pVertex[1].TextureUV = Vector2{ 1.f, 0.f };

	pVertex[2].Position = Vector3{ 1.f, -1.f, 0.f };
	pVertex[2].TextureUV = Vector2{ 1.f, 1.f };

	pVertex[3].Position = Vector3{ -1.f, -1.f, 0.f };
	pVertex[3].TextureUV = Vector2{ 0.f, 1.f };



	INDEX16* pIndex = nullptr;
	m_IndexBuffer->Lock(0, 0, (void**)&pIndex, 0);

	// 오른쪽 위
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	// 왼쪽 아래
	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;



	//normal 값 세팅
	// 오른쪽 위

	Vector3	vDst, vSrc, normal;

	vDst = pVertex[pIndex[0]._1].Position - pVertex[pIndex[0]._0].Position;
	vSrc = pVertex[pIndex[0]._2].Position - pVertex[pIndex[0]._1].Position;
	D3DXVec3Cross(&normal, &vDst, &vSrc);

	pVertex[pIndex[0]._0].Normal += normal;
	pVertex[pIndex[0]._1].Normal += normal;
	pVertex[pIndex[0]._2].Normal += normal;


	// 왼쪽 아래
	vDst = pVertex[pIndex[1]._1].Position - pVertex[pIndex[1]._0].Position;
	vSrc = pVertex[pIndex[1]._2].Position - pVertex[pIndex[1]._1].Position;
	D3DXVec3Cross(&normal, &vDst, &vSrc);

	pVertex[pIndex[1]._0].Normal += normal;
	pVertex[pIndex[1]._1].Normal += normal;
	pVertex[pIndex[1]._2].Normal += normal;


	for (ULONG i = 0; i < m_VertexCnt; ++i)
		D3DXVec3Normalize(&pVertex[i].Normal, &pVertex[i].Normal);


	m_VertexBuffer->Unlock();
	m_IndexBuffer->Unlock();
}

void GameEngine::TextureRenderer::Render(LPDIRECT3DDEVICE9 _device)
{
	//Render State 설정
	_device->SetTransform(D3DTS_WORLD, &Get_Transform().Get_WorldMatrix());

	//shader에 대한 세팅
	//
	if (m_Texture)
		_device->SetTexture(0, m_Texture);

	Render_Buffer(_device);
}


void GameEngine::TextureRenderer::Get_Texture(LPDIRECT3DTEXTURE9& _texture) const
{
	_texture = m_Texture;
}


void GameEngine::TextureRenderer::Set_Texture(const LPDIRECT3DTEXTURE9& _texture)
{
	m_Texture = _texture;
}

