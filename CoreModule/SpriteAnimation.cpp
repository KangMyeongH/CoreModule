#include "SpriteAnimation.h"

#include "RenderManager.h"
#include "Transform.h"

void GameEngine::SpriteAnimation::Load_SpriteSheet()
{
	if (m_Path.empty()) 
		return;

	RenderManager::GetInstance().Add_Texture(m_Path);
	m_Texture = *RenderManager::GetInstance().Get_Texture(m_Path);

	D3DSURFACE_DESC desc;

	HRESULT hr = m_Texture->GetLevelDesc(0, &desc);

	D3DXMatrixScaling(&m_TextureScaleMatrix, static_cast<float>(desc.Width) / 100.f, static_cast<float>(desc.Height) / 100.f, 1.f);
}

void GameEngine::SpriteAnimation::Set_TextureCoordinates(LPDIRECT3DDEVICE9 _device)
{
	int frameX = m_CurrentFrame % m_Columns;
	int frameY = m_CurrentFrame / m_Columns;

	float frameWidthU = 1.0f / static_cast<float>(m_Columns);
	float frameHeightV = 1.0f / static_cast<float>(m_Rows);

	float tuStart 	= static_cast<float>(frameX) * frameWidthU;
	float tvStart 	= static_cast<float>(frameY) * frameHeightV;
	float tuEnd 	= tuStart + frameWidthU;
	float tvEnd 	= tvStart + frameHeightV;

	D3DXMATRIX textureMat = {
		(tuEnd - tuStart), 	0.0f,				0.0f, 0.0f,
		0.0f, 				(tvEnd - tvStart), 	0.0f, 0.0f,
		0.0f, 				0.0f,				1.0f, 0.0f,
		tuStart, 			tvStart,			0.0f, 1.0f
	};

	_device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

	_device->SetTransform(D3DTS_TEXTURE0, &textureMat);
}

void GameEngine::SpriteAnimation::Update_Frame(float _deltaTime)
{
	m_AccTime += _deltaTime;

	if (m_AccTime >= m_DurationTime)
	{
		m_CurrentFrame = (m_CurrentFrame + 1) % m_TotalFrames;
		m_AccTime = 0.0f;
	}
}

void GameEngine::SpriteAnimation::Ready_Buffer(LPDIRECT3DDEVICE9 _device)
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

void GameEngine::SpriteAnimation::Render(LPDIRECT3DDEVICE9 _device)
{
	D3DXMATRIX worldMat = m_TextureScaleMatrix * Get_Transform().Get_WorldMatrix();
	_device->SetTransform(D3DTS_WORLD, &worldMat);


}

GameEngine::Component* GameEngine::SpriteAnimation::Clone() const
{
	return new SpriteAnimation(*this);
}

void GameEngine::SpriteAnimation::to_json(nlohmann::ordered_json& _j)
{
}

void GameEngine::SpriteAnimation::from_json(const nlohmann::ordered_json& _j)
{
}
