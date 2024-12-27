#include "RenderManager.h"

#include "GameObject.h"
#include "Renderer.h"
#include "Scene.h"

IMPLEMENT_SINGLETON(GameEngine::RenderManager)

void GameEngine::RenderManager::Ready_Buffer(LPDIRECT3DDEVICE9 _device)
{
	for (auto& renderer : m_RegisterQueue)
	{
		dynamic_cast<CubeRenderer*>(renderer)->Ready_Buffer(_device);
	}
}

GameEngine::RenderManager::~RenderManager()
{
}

void GameEngine::RenderManager::Render_Begin(LPDIRECT3DDEVICE9 _device)
{
	D3DXCOLOR backColor = { 1.f, 1.f, 1.f, 1.f };
	_device->Clear(0,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,
		backColor,
		1.f,
		0);

	_device->BeginScene();
}

void GameEngine::RenderManager::Render(LPDIRECT3DDEVICE9 _device)
{
	if (FAILED(_device->SetTransform(D3DTS_VIEW, &m_ViewMat)))
	{
		return;
	}
	if (FAILED(_device->SetTransform(D3DTS_PROJECTION, &m_ProjMat)))
	{
		return;
	}

	for (const auto& renderer : m_Renderers)
	{
		renderer->Render(_device);
	}
}

void GameEngine::RenderManager::Render_End(LPDIRECT3DDEVICE9 _device)
{
	_device->EndScene();
	_device->Present(NULL, NULL, NULL, NULL);
}

void GameEngine::RenderManager::Add_Renderer(Renderer* _renderer)
{
	m_RegisterQueue.push_back(_renderer);
}

void GameEngine::RenderManager::Remove_Renderer(Renderer* _renderer)
{
	for (auto& renderer : m_DestroyQueue)
	{
		if (renderer == _renderer)
		{
			return;
		}
	}

	m_DestroyQueue.push_back(_renderer);
}

void GameEngine::RenderManager::Register_Renderer()
{
	for (auto it = m_RegisterQueue.begin(); it != m_RegisterQueue.end();)
	{
		Renderer* renderer = *it;

		if (renderer->Is_Enabled())
		{
			m_Renderers.push_back(renderer);

			it = m_RegisterQueue.erase(it);
		}

		else ++it;
	}
}

void GameEngine::RenderManager::Destroy_Renderer()
{
	for (auto& renderer : m_DestroyQueue)
	{
		GameObjectList* objList = Scene::GetInstance().Get_GameObjectList();
		for (auto& gameObject : *objList)
		{
			if (gameObject == renderer->Get_GameObject())
			{
				gameObject->Remove_Component(renderer);
				break;
			}
		}

		delete renderer;
		m_Renderers.erase(std::remove(m_Renderers.begin(), m_Renderers.end(), renderer), m_Renderers.end());
	}

	m_DestroyQueue.clear();
}

void GameEngine::RenderManager::Release()
{
	for (const auto& renderer : m_Renderers)
	{
		delete renderer;
	}

	for (const auto& renderer : m_RegisterQueue)
	{
		delete renderer;
	}

	m_Renderers.clear();
	m_RegisterQueue.clear();
	m_DestroyQueue.clear();
}
