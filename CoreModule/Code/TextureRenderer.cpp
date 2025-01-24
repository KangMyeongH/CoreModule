#include "TextureRenderer.h"

#include "GameObject.h"
#include "Material.h"
#include "Transform.h"

void GameEngine::TextureRenderer::Ready_Buffer(LPDIRECT3DDEVICE9 _device)
{
	m_TriangleCnt = 2;
	m_VertexCnt = 4;
	m_VertexSize = sizeof(VTXTEX);
	m_FVF = FVF_TEX;
	UINT indexSize = sizeof(INDEX16);

	if (E_FAIL == _device->CreateVertexBuffer(
		m_VertexCnt * m_VertexSize,
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

void GameEngine::TextureRenderer::Render(LPDIRECT3DDEVICE9 _device)
{
	//Render State 설정
	D3DXMATRIX flipMat;
	Compute_FlipMat(flipMat);
	D3DXMATRIX  world = flipMat * m_TextureScaleMatrix * Get_Transform().Get_WorldMatrix();

	if (m_pMaterial)
	{
		D3DXMATRIX viewMat = RenderManager::GetInstance().Get_ViewMat();
		D3DXMATRIX projMat = RenderManager::GetInstance().Get_ProjMat();

		D3DXMATRIX finalWorld = world;

		if (m_bBillboard)
		{
			finalWorld = Make_BillboardMatrix(world, viewMat);
		}

		m_pMaterial->Set_Light(RenderManager::GetInstance().Get_Light()->Get_LightInfo());
		m_pMaterial->Set_Color("gAmbientColor", D3DXVECTOR4(m_Material.Ambient.r, m_Material.Ambient.g, m_Material.Ambient.b, m_Material.Ambient.a));
		m_pMaterial->Set_Color("gMaterialColor", D3DXVECTOR4(m_Material.Diffuse.r, m_Material.Diffuse.g, m_Material.Diffuse.b, m_Material.Diffuse.a));
		m_pMaterial->Set_WorldMat(finalWorld);
		m_pMaterial->Set_ViewProjMat(viewMat, projMat);
		m_pMaterial->Set_Float("gBlinkAlpha", m_BlinkAlpha);
		m_pMaterial->Set_Texture("gDiffuseMap", m_Texture);
		m_pMaterial->Set_Color("gBlinkColor", D3DXVECTOR4(1.f, 1.f, 1.f, 1.f));

		if (m_bDither)
		{
			if (GameObject* target = GameObject::Find("Player"))
			{
				auto playerPos = target->Get_Transform().Position();
				auto myPos = Get_Transform().Position();

				// 우선 z축 비교
				if (playerPos.z < myPos.z)
				{
					// 플레이어가 오브젝트 '앞쪽'에 있음
					m_DitherFactor = 0.0f;
				}
				else
				{
					// 그렇지 않을 때는 거리 기반 보간 로직
					float dist = (playerPos - myPos).Magnitude();  // 플레이어와의 거리
					float nearDist = 5.0f;
					float farDist = 10.0f;

					// dist가 1 이하 -> factor=1, dist가 5 이상 -> factor=0, 그 사이 보간
					float factor = (farDist - dist) / (farDist - nearDist);
					if (factor < 0.f) factor = 0.f;
					if (factor > 1.f) factor = 1.f;

					m_DitherFactor = factor;
				}
			}
			m_pMaterial->Set_Float("gDitherFactor", m_DitherFactor);

			m_pMaterial->Get_Effect()->SetTechnique("DirLightDither");
			UINT passCount = 0;
			m_pMaterial->Get_Effect()->Begin(&passCount, 0);
		}

		else
		{
			if (m_bOutline)
			{
				m_pMaterial->Get_Effect()->SetTechnique("OutlineTech");
				UINT passCount = 0;
				m_pMaterial->Get_Effect()->Begin(&passCount, 0);
				m_pMaterial->Begin_Pass(0);

				_device->SetStreamSource(0, m_VertexBuffer, 0, m_VertexSize);
				_device->SetFVF(m_FVF);

				_device->SetIndices(m_IndexBuffer);
				_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_VertexCnt, 0, m_TriangleCnt);

				m_pMaterial->End_Pass();
				m_pMaterial->End();
			}

			m_pMaterial->Begin();
		}

		m_pMaterial->Begin_Pass(0);

		//Buffer 출력
		_device->SetStreamSource(0, m_VertexBuffer, 0, m_VertexSize);
		_device->SetFVF(m_FVF);

		_device->SetIndices(m_IndexBuffer);
		_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_VertexCnt, 0, m_TriangleCnt);

		m_pMaterial->End_Pass();
		m_pMaterial->End();

		return;
	}

	_device->SetTransform(D3DTS_WORLD, &world);

	//texture 세팅
	_device->SetTexture(0, m_Texture);

	Render_Buffer(_device);
}

void GameEngine::TextureRenderer::Compute_FlipMat(D3DXMATRIX& _mat) const
{
	Vector3 flipScale = Vector3{ 1.f, 1.f, 1.f };

	if (m_FlipX)
		flipScale.x *= -1.f;
	if (m_FlipY)
		flipScale.y *= -1.f;
	D3DXMatrixScaling(&_mat, flipScale.x, flipScale.y, 1.f);
}

void GameEngine::TextureRenderer::Set_NativeSize()
{
	if (!m_Texture)
		return;

	D3DXIMAGE_INFO imageInfo;
	D3DXGetImageInfoFromFile(m_Path.c_str(), &imageInfo);
	D3DXMatrixScaling(&m_TextureScaleMatrix, static_cast<float>(imageInfo.Width) / 100.f, static_cast<float>(imageInfo.Height) / 100.f, 1.f);
}

void GameEngine::TextureRenderer::Set_NormalSize()
{
	D3DXMatrixScaling(&m_TextureScaleMatrix, 1.f, 1.f, 1.f);
}

void GameEngine::TextureRenderer::Set_Material(Material* _material)
{
	m_pMaterial = _material;
}

//
//void GameEngine::TextureRenderer::Get_Texture(LPDIRECT3DTEXTURE9& _texture) const
//{
//	_texture = m_Texture;
//}
//
//
//void GameEngine::TextureRenderer::Set_Texture(const LPDIRECT3DTEXTURE9& _texture)
//{
//	m_Texture = _texture;
//}

