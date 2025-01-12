#include "UIManager.h"
#include "TextureUI.h"
#include "GameObject.h"
#include "Scene.h"
#include "UI.h"

IMPLEMENT_SINGLETON(GameEngine::UIManager)

void GameEngine::UIManager::Initialize(LPDIRECT3DDEVICE9 _device)
{
	m_Device = _device;
	_device->AddRef();

	GameEngine::CUSTOM_VERTEX vertices[] =
	{
		// 첫 번째 삼각형 (왼쪽 하단, 왼쪽 상단, 오른쪽 하단)
		{ D3DXVECTOR3(-0.5f, -0.5f, 0.0f), Vector2(0.0f, 1.0f) }, // 왼쪽 하단
		{ D3DXVECTOR3(-0.5f,  0.5f, 0.0f), Vector2(0.0f, 0.0f) }, // 왼쪽 상단
		{ D3DXVECTOR3(0.5f, -0.5f, 0.0f), Vector2(1.0f, 1.0f) },  // 오른쪽 하단

		// 두 번째 삼각형 (오른쪽 하단, 왼쪽 상단, 오른쪽 상단)
		{ D3DXVECTOR3(0.5f, -0.5f, 0.0f), Vector2(1.0f, 1.0f) },  // 오른쪽 하단
		{ D3DXVECTOR3(-0.5f,  0.5f, 0.0f), Vector2(0.0f, 0.0f) }, // 왼쪽 상단
		{ D3DXVECTOR3(0.5f,  0.5f, 0.0f), Vector2(1.0f, 0.0f) }   // 오른쪽 상단
	};

	// 정점 버퍼 생성
	HRESULT hr = _device->CreateVertexBuffer
	(
		sizeof(vertices),              // 버퍼 크기
		D3DUSAGE_WRITEONLY,            // 쓰기 전용
		GameEngine::FVF_UITEX,         // 정점 형식
		D3DPOOL_MANAGED,               // 메모리 풀
		&m_pVertexBuffer,               // 버텍스 버퍼 포인터
		NULL                           // 반환할 물리적 메모리 포인터 없음
	);

	// 버텍스 버퍼에 데이터 복사
	VOID* pVoid;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	m_pVertexBuffer->Unlock();
}

void GameEngine::UIManager::Render_UI()
{
	D3DXMATRIX matProj, matView, matWorld;

	// 1. 직교 투영 행렬 설정
	D3DXMatrixOrthoLH(&matProj, 1920, 1080, 0.0f, 1.0f);
	m_Device->SetTransform(D3DTS_PROJECTION, &matProj);

	// 2. 뷰 행렬 설정 (Identity)
	D3DXMatrixIdentity(&matView);
	m_Device->SetTransform(D3DTS_VIEW, &matView);

	m_Device->SetMaterial(nullptr);
	m_Device->SetRenderState(D3DRS_LIGHTING, false);

	m_Device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

	//불투명 객체 렌더
	m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE); // 알파 블렌딩 활성화
	m_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);   // 알파 테스트 활성화
	m_Device->SetRenderState(D3DRS_ALPHAREF, 128);          // 알파값 128 기준
	m_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);     // Z 버퍼 쓰기 활성화

	for (auto textureUI : m_TextureUI[UI::ALPHA_RENDERING])
	{
		if (textureUI->Is_Enabled() && textureUI->Get_GameObject()->Is_Active())
		{
			textureUI->Render_Texture(m_Device);
		}
	}

	//반투명 객체 렌더
	m_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);  // 알파 테스트 비활성화
	m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); // 알파 블렌딩 활성화
	m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);     // Z 버퍼 쓰기 비활성화

	//std::sort(m_Renderers[1].begin(), m_Renderers[1].end(), [&_device](Renderer* dst, Renderer* src)->bool
	//	{
	//		D3DXMATRIX matCameraWorld;
	//
	//		_device->GetTransform(D3DTS_VIEW, &matCameraWorld);
	//		D3DXMatrixInverse(&matCameraWorld, 0, &matCameraWorld);
	//
	//		Vector3   cameraPosition;
	//		memcpy(&cameraPosition, &matCameraWorld.m[3][0], sizeof(Vector3));
	//
	//		Vector3 dstZ = cameraPosition - dst->Get_Transform().Position();
	//		Vector3	srcZ = cameraPosition - src->Get_Transform().Position();
	//
	//		float dstLength = D3DXVec3Length(&dstZ);
	//		float srcLength = D3DXVec3Length(&srcZ);
	//
	//		return dstLength > srcLength;
	//	});

	for (auto textureUI : m_TextureUI[UI::ALPHA_BLENDING])
	{
		if (textureUI->Is_Enabled() && textureUI->Get_GameObject()->Is_Active())
		{
			textureUI->Render_Texture(m_Device);
		}
	}
}

void GameEngine::UIManager::Add_TextureUI(TextureUI* _textureUI)
{
	_textureUI->Ready_Buffer(m_Device, m_pVertexBuffer);
	m_pVertexBuffer->AddRef();

	_textureUI->Update_Texture();

	m_RegisterQueue.push_back(_textureUI);
}

void GameEngine::UIManager::Remove_Renderer(TextureUI* _textureUI)
{
	for (auto& texture : m_DestroyQueue)
	{
		if (texture == _textureUI)
		{
			return;
		}
	}

	m_DestroyQueue.push_back(_textureUI);
}

void GameEngine::UIManager::Register_UI()
{
	for (auto it = m_RegisterQueue.begin(); it != m_RegisterQueue.end();)
	{
		TextureUI* textureUI = *it;

		if (textureUI->Is_Enabled())
		{
			if (textureUI->Get_RenderOption() == UI::ALPHA_RENDERING)
				m_TextureUI[UI::ALPHA_RENDERING].push_back(textureUI);
			else if (textureUI->Get_RenderOption() == UI::ALPHA_BLENDING)
				m_TextureUI[UI::ALPHA_BLENDING].push_back(textureUI);

			it = m_RegisterQueue.erase(it);
		}

		else ++it;
	}
}

void GameEngine::UIManager::Destroy_UI()
{
	for (auto& textureUI : m_DestroyQueue)
	{
		GameObjectList* objList = Scene::GetInstance().Get_GameObjectList();
		for (auto& gameObject : *objList)
		{
			if (gameObject == textureUI->Get_GameObject())
			{
				gameObject->Remove_Component(textureUI);
				break;
			}
		}

		delete textureUI;
		m_TextureUI[UI::ALPHA_RENDERING].erase(std::remove(m_TextureUI[UI::ALPHA_RENDERING].begin(), m_TextureUI[UI::ALPHA_RENDERING].end(), textureUI), m_TextureUI[UI::ALPHA_RENDERING].end());
		m_TextureUI[UI::ALPHA_BLENDING].erase(std::remove(m_TextureUI[UI::ALPHA_BLENDING].begin(), m_TextureUI[UI::ALPHA_BLENDING].end(), textureUI), m_TextureUI[UI::ALPHA_BLENDING].end());
	}

	m_DestroyQueue.clear();
}

void GameEngine::UIManager::Clear_Component()
{
	for (const auto& rendererList : m_TextureUI)
	{
		for (const auto& renderer : rendererList)
		{
			delete renderer;
		}
	}

	for (const auto& renderer : m_RegisterQueue)
	{
		delete renderer;
	}

	m_TextureUI[0].clear();
	m_TextureUI[1].clear();

	m_RegisterQueue.clear();
	m_DestroyQueue.clear();
}

void GameEngine::UIManager::Release()
{
	Clear_Component();
	m_pVertexBuffer->Release();
}
