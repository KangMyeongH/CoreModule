#include "CubeRenderer.h"

#include "GameObject.h"
#include "Transform.h"

//vertex buffer, index buffer 초기화
void GameEngine::CubeRenderer::Ready_Buffer(LPDIRECT3DDEVICE9 _device)
{
	m_TriangleCnt = 12;
	m_VertexCnt = 24;
	m_VertexSize = sizeof(VTXCUBE);
	m_FVF = FVF_COLOR;
	UINT indexSize = sizeof(INDEX16);

	if (E_FAIL == _device->CreateVertexBuffer(m_VertexCnt * m_VertexSize,
		0,
		m_FVF,
		D3DPOOL_MANAGED,
		&m_VertexBuffer,
	nullptr))
		return;

	if(E_FAIL == _device->CreateIndexBuffer(m_TriangleCnt * indexSize,
		0,						
		D3DFMT_INDEX16,			
		D3DPOOL_MANAGED,
		&m_IndexBuffer,
	nullptr))
		return;

	VTXCUBE* pVertex = nullptr;

	m_VertexBuffer->Lock(0, 0, (void**)&pVertex, 0);

	//normal vector 추가 전 vertex setting
	// 전면
	pVertex[0].Position = Vector3{ -1.f, -1.f, -1.f };
	pVertex[1].Position = Vector3{ 1.f, -1.f, -1.f };
	pVertex[2].Position = Vector3{ 1.f, 1.f, -1.f };
	pVertex[3].Position = Vector3{ -1.f, 1.f, -1.f };

	pVertex[0].Normal = Vector3{0.f, 0.f, -1.f};
	pVertex[1].Normal = Vector3{ 0.f, 0.f, -1.f };
	pVertex[2].Normal = Vector3{ 0.f, 0.f, -1.f };
	pVertex[3].Normal = Vector3{ 0.f, 0.f, -1.f };


	// 후면
	pVertex[4].Position = Vector3{ 1.f, -1.f, 1.f };
	pVertex[5].Position = Vector3{ -1.f, -1.f, 1.f };
	pVertex[6].Position = Vector3{ -1.f, 1.f, 1.f };
	pVertex[7].Position = Vector3{ 1.f, 1.f, 1.f };

	pVertex[4].Normal = Vector3{ 0.f, 0.f, 1.f };
	pVertex[5].Normal = Vector3{ 0.f, 0.f, 1.f };
	pVertex[6].Normal = Vector3{ 0.f, 0.f, 1.f };
	pVertex[7].Normal = Vector3{ 0.f, 0.f, 1.f };

	// 좌측
	pVertex[8].Position = Vector3{ -1.f, -1.f, 1.f };
	pVertex[9].Position = Vector3{ -1.f, -1.f, -1.f };
	pVertex[10].Position = Vector3{ -1.f, 1.f, -1.f };
	pVertex[11].Position = Vector3{ -1.f, 1.f, 1.f };

	pVertex[8].Normal = Vector3{ -1.f, 0.f, 0.f };
	pVertex[9].Normal = Vector3{ -1.f, 0.f, 0.f };
	pVertex[10].Normal = Vector3{ -1.f, 0.f, 0.f };
	pVertex[11].Normal = Vector3{ -1.f, 0.f, 0.f };

	// 우측
	pVertex[12].Position = Vector3{ 1.f, -1.f, -1.f };
	pVertex[13].Position = Vector3{ 1.f, -1.f, 1.f };
	pVertex[14].Position = Vector3{ 1.f, 1.f, 1.f };
	pVertex[15].Position = Vector3{ 1.f, 1.f, -1.f };

	pVertex[12].Normal = Vector3{ 1.f, 0.f, 0.f };
	pVertex[13].Normal = Vector3{ 1.f, 0.f, 0.f };
	pVertex[14].Normal = Vector3{ 1.f, 0.f, 0.f };
	pVertex[15].Normal = Vector3{ 1.f, 0.f, 0.f };

	// 상
	pVertex[16].Position = Vector3{ -1.f, 1.f, -1.f };
	pVertex[17].Position = Vector3{ 1.f, 1.f, -1.f };
	pVertex[18].Position = Vector3{ 1.f, 1.f, 1.f };
	pVertex[19].Position = Vector3{ -1.f, 1.f, 1.f };

	pVertex[16].Normal = Vector3{ 0.f, 1.f, 0.f };
	pVertex[17].Normal = Vector3{ 0.f, 1.f, 0.f };
	pVertex[18].Normal = Vector3{ 0.f, 1.f, 0.f };
	pVertex[19].Normal = Vector3{ 0.f, 1.f, 0.f };

	// 하
	pVertex[20].Position = Vector3{ -1.f, -1.f, 1.f };
	pVertex[21].Position = Vector3{ 1.f, -1.f, 1.f };
	pVertex[22].Position = Vector3{ 1.f, -1.f, -1.f };
	pVertex[23].Position = Vector3{ -1.f, -1.f, -1.f };

	pVertex[20].Position = Vector3{ 0.f, -1.f, 0.f };
	pVertex[21].Position = Vector3{ 0.f, -1.f, 0.f };
	pVertex[22].Position = Vector3{ 0.f, -1.f, 0.f };
	pVertex[23].Position = Vector3{ 0.f, -1.f, 0.f };
	
	
	for (UINT i = 0; i < m_VertexCnt; i++)
	{
		pVertex[i].Color = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	}


	INDEX16* pIndex = nullptr;

	m_IndexBuffer->Lock(0, 0, (void**)&pIndex, 0);

	// 정면
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 2;
	pIndex[0]._2 = 1;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 3;
	pIndex[1]._2 = 2;

	//후면
	pIndex[2]._0 = 4;
	pIndex[2]._1 = 6;
	pIndex[2]._2 = 5;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 7;
	pIndex[3]._2 = 6;

	// 좌측
	pIndex[4]._0 = 8;
	pIndex[4]._1 = 10;
	pIndex[4]._2 = 9;

	pIndex[5]._0 = 8;
	pIndex[5]._1 = 11;
	pIndex[5]._2 = 10;

	// 우측
	pIndex[6]._0 = 12;
	pIndex[6]._1 = 14;
	pIndex[6]._2 = 13;

	pIndex[7]._0 = 12;
	pIndex[7]._1 = 15;
	pIndex[7]._2 = 14;

	// 위
	pIndex[8]._0 = 16;
	pIndex[8]._1 = 18;
	pIndex[8]._2 = 17;

	pIndex[9]._0 = 16;
	pIndex[9]._1 = 19;
	pIndex[9]._2 = 18;

	// 아래
	pIndex[10]._0 = 20;
	pIndex[10]._1 = 22;
	pIndex[10]._2 = 21;

	pIndex[11]._0 = 20;
	pIndex[11]._1 = 23;
	pIndex[11]._2 = 22;


	m_VertexBuffer->Unlock();
	m_IndexBuffer->Unlock();
}

void GameEngine::CubeRenderer::Render(LPDIRECT3DDEVICE9 _device)
{
	//Transfrom 설정
	_device->SetTransform(D3DTS_WORLD, &Get_Transform().Get_WorldMatrix());

	Render_Buffer(_device);
	//render state 복구

}



