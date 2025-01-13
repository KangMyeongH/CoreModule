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
		FVF_UITEX,         // 정점 형식
		D3DPOOL_MANAGED,               // 메모리 풀
		&m_VertexBuffer,               // 버텍스 버퍼 포인터
		nullptr                           // 반환할 물리적 메모리 포인터 없음
	);

	// 버텍스 버퍼에 데이터 복사
	VOID* pVoid;
	m_VertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	m_VertexBuffer->Unlock();
}

void GameEngine::UIManager::Render_UI()
{
	D3DXMATRIX matProj, matView;

	// 1. 직교 투영 행렬 설정
	D3DXMatrixOrthoLH(&matProj, 1920, 1080, -1.0f, 1.0f);
	m_Device->SetTransform(D3DTS_PROJECTION, &matProj);

	// 2. 뷰 행렬 설정 (Identity)
	D3DXMatrixIdentity(&matView);
	m_Device->SetTransform(D3DTS_VIEW, &matView);

	m_Device->SetRenderState(D3DRS_LIGHTING, false);

	m_Device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

	m_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

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
			textureUI->Render_UI(m_Device);
		}
	}

	//반투명 객체 렌더
	m_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);  // 알파 테스트 비활성화
	m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); // 알파 블렌딩 활성화
	m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);     // Z 버퍼 쓰기 비활성화

	for (auto textureUI : m_TextureUI[UI::ALPHA_BLENDING])
	{
		if (textureUI->Is_Enabled() && textureUI->Get_GameObject()->Is_Active())
		{
			textureUI->Render_UI(m_Device);
		}
	}

	m_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void GameEngine::UIManager::Add_UI(UI* _ui)
{
	m_RegisterQueue.push_back(_ui);
}

void GameEngine::UIManager::Remove_Renderer(UI* _ui)
{
	for (auto& texture : m_DestroyQueue)
	{
		if (texture == _ui)
		{
			return;
		}
	}

	m_DestroyQueue.push_back(_ui);
}

void GameEngine::UIManager::Register_UI()
{
	for (auto it = m_RegisterQueue.begin(); it != m_RegisterQueue.end();)
	{
		UI* _ui = *it;

		if (_ui->Is_Enabled())
		{
			if (_ui->Get_RenderOption() == UI::ALPHA_RENDERING)
			{
				m_TextureUI[UI::ALPHA_RENDERING].push_back(_ui);
				_ui->Ready_UI(m_Device);
			}

			else if (_ui->Get_RenderOption() == UI::ALPHA_BLENDING)
			{
				m_TextureUI[UI::ALPHA_BLENDING].push_back(_ui);
				_ui->Ready_UI(m_Device);
			}

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
	m_VertexBuffer->Release();
}
