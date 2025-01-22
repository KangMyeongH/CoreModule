#include "UIManager.h"
#include "TextureUI.h"
#include "GameObject.h"
#include "Scene.h"
#include "TimeManager.h"
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

	m_FadeVertex[0] = { D3DXVECTOR3(-960.0f,540.0f,0.0f), D3DCOLOR_ARGB(255, 0,0,0) };
	m_FadeVertex[1] = { D3DXVECTOR3(960.f, 540.0f, 0.0f), D3DCOLOR_ARGB(255, 0, 0, 0) };
	m_FadeVertex[2] = { D3DXVECTOR3(-960.f, -540.f, 0.0f), D3DCOLOR_ARGB(255, 0, 0, 0) };
	m_FadeVertex[3] = { D3DXVECTOR3(960.f, -540.f, 0.0f), D3DCOLOR_ARGB(255, 0, 0, 0) };

	RenderManager::GetInstance().Add_Texture(L"..\\Client\\Assets\\Resource\\Texture\\Loading\\LoadingIcon.png");
	D3DXIMAGE_INFO iconInfo;
	D3DXGetImageInfoFromFile(L"..\\Client\\Assets\\Resource\\Texture\\Loading\\LoadingIcon.png", &iconInfo);
	D3DXMatrixScaling(&m_IconScale, static_cast<float>(iconInfo.Width), static_cast<float>(iconInfo.Height), 1.f);

	RenderManager::GetInstance().Add_Texture(L"..\\Client\\Assets\\Resource\\Texture\\Loading\\Pentagram-Star_1.png");
	D3DXIMAGE_INFO starInfo;
	D3DXGetImageInfoFromFile(L"..\\Client\\Assets\\Resource\\Texture\\Loading\\Pentagram-Star_1.png", &starInfo);
	D3DXMatrixScaling(&m_StarScale, static_cast<float>(starInfo.Width), static_cast<float>(starInfo.Height), 1.f);

	m_IconTransform.Set_Position(Vector3(-840.f, -440.f, 0.f));
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

	for (auto ui : mUI[UI::ALPHA_RENDERING])
	{
		if (ui->Is_Enabled() && ui->Get_GameObject()->Is_Active())
		{
			ui->Render_UI(m_Device);
		}
	}

	//반투명 객체 렌더
	m_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);  // 알파 테스트 비활성화
	m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); // 알파 블렌딩 활성화
	m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);     // Z 버퍼 쓰기 비활성화

	for (auto ui : mUI[UI::ALPHA_BLENDING])
	{
		if (ui->Is_Enabled() && ui->Get_GameObject()->Is_Active())
		{
			ui->Render_UI(m_Device);
		}
	}

	if (m_bFadeIn || m_bFadeOut)
	{
		m_Device->SetRenderState(D3DRS_ZENABLE, FALSE);

		update_FadeEffect();
		render_FadeEffect();

		m_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
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
				mUI[UI::ALPHA_RENDERING].push_back(_ui);
				_ui->Ready_UI(m_Device);
			}

			else if (_ui->Get_RenderOption() == UI::ALPHA_BLENDING)
			{
				mUI[UI::ALPHA_BLENDING].push_back(_ui);
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
		mUI[UI::ALPHA_RENDERING].erase(std::remove(mUI[UI::ALPHA_RENDERING].begin(), mUI[UI::ALPHA_RENDERING].end(), textureUI), mUI[UI::ALPHA_RENDERING].end());
		mUI[UI::ALPHA_BLENDING].erase(std::remove(mUI[UI::ALPHA_BLENDING].begin(), mUI[UI::ALPHA_BLENDING].end(), textureUI), mUI[UI::ALPHA_BLENDING].end());
	}

	m_DestroyQueue.clear();
}

void GameEngine::UIManager::Clear_Component()
{
	for (const auto& rendererList : mUI)
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

	mUI[0].clear();
	mUI[1].clear();

	m_RegisterQueue.clear();
	m_DestroyQueue.clear();
}

void GameEngine::UIManager::Release()
{
	Clear_Component();
	m_VertexBuffer->Release();
}

void GameEngine::UIManager::Render_LoadingScreen()
{
	LPDIRECT3DTEXTURE9 icon = *RenderManager::GetInstance().Get_Texture(L"..\\Client\\Assets\\Resource\\Texture\\Loading\\LoadingIcon.png");
	LPDIRECT3DTEXTURE9 star = *RenderManager::GetInstance().Get_Texture(L"..\\Client\\Assets\\Resource\\Texture\\Loading\\Pentagram-Star_1.png");

	float rotate = 360.f * 0.01667f;
	 
	Vector3 rotation = m_IconTransform.Get_LocalRotation() + Vector3(0.f, 0.f, rotate);
	m_IconTransform.Set_LocalRotation(rotation);

	D3DXMATRIX iconMat = m_IconScale * m_IconTransform.Get_WorldMatrix();
	D3DXMATRIX starMat = m_StarScale * m_StarTransform.Get_WorldMatrix();

	m_Device->Clear(0,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0,0,0),
		1.f,
		0);

	m_Device->BeginScene();

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

	//반투명 객체 렌더
	m_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);  // 알파 테스트 비활성화
	m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); // 알파 블렌딩 활성화
	m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);     // Z 버퍼 쓰기 비활성화

	m_Device->SetTransform(D3DTS_WORLD, &iconMat);
	m_Device->SetTexture(0, icon);
	m_Device->SetFVF(FVF_UITEX);
	m_Device->SetStreamSource(0, m_VertexBuffer, 0, sizeof(CUSTOM_VERTEX));
	m_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	m_Device->SetTransform(D3DTS_WORLD, &starMat);
	m_Device->SetTexture(0, star);
	m_Device->SetFVF(FVF_UITEX);
	m_Device->SetStreamSource(0, m_VertexBuffer, 0, sizeof(CUSTOM_VERTEX));
	m_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	m_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_Device->EndScene();
	m_Device->Present(nullptr, nullptr, nullptr, nullptr);
}

void GameEngine::UIManager::update_FadeEffect()
{
	if (m_bFadeIn)
	{
		m_FadeAlpha -= m_FadeSpeed * TimeManager::GetInstance().Get_DeltaTime(); // 투명도를 줄임
		if (m_FadeAlpha <= 0.0f) 
		{
			m_FadeAlpha = 0.0f; // 완전 투명
			m_bFadeIn = false; // 페이드 완료
			m_bFadeInFinish = true;
		}
	}

	else if (m_bFadeOut)
	{
		m_FadeAlpha += m_FadeSpeed * TimeManager::GetInstance().Get_DeltaTime(); // 투명도를 높임
		if (m_FadeAlpha >= 1.0f) 
		{
			m_FadeAlpha = 1.0f; // 완전 불투명
			m_bFadeOutFinish = true;
		}
	}
}

void GameEngine::UIManager::render_FadeEffect()
{
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	m_Device->SetTransform(D3DTS_WORLD, &mat);
	m_Device->SetTexture(0, nullptr);
	DWORD alpha = (DWORD)(m_FadeAlpha * 255.f);
	for (int i = 0; i < 4; ++i)
	{
		m_FadeVertex[i].color = D3DCOLOR_ARGB(alpha, 0, 0, 0);
	}

	m_Device->SetFVF(D3DFVF_FADEVERTEX);
	m_Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_FadeVertex, sizeof(FADE_VERTEX));
}
