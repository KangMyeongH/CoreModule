#include "MultiTextureRenderer.h"

void GameEngine::MultiTextureRenderer::Ready_Buffer(LPDIRECT3DDEVICE9 _device)
{
	m_TriangleCnt = 2;
	m_VertexCnt = 4;
	m_VertexSize = sizeof(VTXTEX);
	m_FVF = FVF_TEX;
	UINT indexSize = sizeof(INDEX16);

	if (E_FAIL == _device->CreateVertexBuffer(m_VertexCnt * m_VertexSize,
		0,
		m_FVF,
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

	pVertex[0].Position = Vector3{ -0.5f, 0.5f, 0.f };
	pVertex[0].Normal = Vector3{ 0.0f, 0.0f, 1.0f };
	pVertex[0].TextureUV = Vector2{ 0.f, 0.f };

	pVertex[1].Position = Vector3{ 0.5f, 0.5f, 0.f };
	pVertex[1].Normal = Vector3{ 0.0f, 0.0f, 1.0f };
	pVertex[1].TextureUV = Vector2{ 1.f, 0.f };

	pVertex[2].Position = Vector3{ 0.5f, -0.5f, 0.f };
	pVertex[2].Normal = Vector3{ 0.0f, 0.0f, 1.0f };
	pVertex[2].TextureUV = Vector2{ 1.f, 1.f };

	pVertex[3].Position = Vector3{ -0.5f, -0.5f, 0.f };
	pVertex[3].Normal = Vector3{ 0.0f, 0.0f, 1.0f };
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


	m_VertexBuffer->Unlock();
	m_IndexBuffer->Unlock();
}

void GameEngine::MultiTextureRenderer::Render(LPDIRECT3DDEVICE9 _device)
{
	//Render State 설정

	D3DXMATRIX flipMat;
	Compute_FlipMat(flipMat);

	D3DXMATRIX  world = flipMat * m_TextureScaleMatrix * Get_Transform().Get_WorldMatrix();
	_device->SetTransform(D3DTS_WORLD, &world);

	//cullmode 변경을 매 renderer마다 껐다 키면 부하가 심함. 같은 애들끼리 묶어서 출력하게 변경해야 함
	_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (m_Texture == nullptr)
	{
		//texture 세팅
		_device->SetTexture(0, nullptr);
	}

	else
	{
		//texture 세팅
		_device->SetTexture(0, (*m_Texture)[m_Frame]);
	}

	Render_Buffer(_device);

	_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void GameEngine::MultiTextureRenderer::Compute_FlipMat(D3DXMATRIX& _mat) const
{
	Vector3 flipScale = Vector3{ 1.f, 1.f, 1.f };

	if (m_FlipX)
		flipScale.x *= -1.f;
	if (m_FlipY)
		flipScale.y *= -1.f;
	D3DXMatrixScaling(&_mat, flipScale.x, flipScale.y, 1.f);
}

bool GameEngine::MultiTextureRenderer::Set_Frame(const std::wstring& _path, int _frame)
{

	if (m_Path != _path)
		m_Texture = RenderManager::GetInstance().Get_MultiTexture(_path);

	m_Path = _path;

	if (!m_Texture)
		return false;

	if (_frame < 0 || _frame >= m_Texture->size())
		return false;

	m_Frame = _frame;

	Set_NativeSize();

	return true;
}

void GameEngine::MultiTextureRenderer::Set_NativeSize()
{
	if (!m_Texture)
		return;

	if (m_Frame < 0 || m_Frame >= m_Texture->size())
		return;

	D3DSURFACE_DESC desc;

	HRESULT hr = (*m_Texture)[m_Frame]->GetLevelDesc(0, &desc);
	if (SUCCEEDED(hr)) {
		D3DXMatrixScaling(&m_TextureScaleMatrix, float(desc.Width) / 100.f, float(desc.Height) / 100.f, 1.f);
	}
}

void GameEngine::MultiTextureRenderer::Set_NormalSize()
{
	D3DXMatrixScaling(&m_TextureScaleMatrix, 1.f, 1.f, 1.f);
}
