#include "RenderManager.h"

IMPLEMENT_SINGLETON(GameEngine::RenderManager)

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
	for (Renderer* renderer : m_Renderers)
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
	m_Renderers.push_back(_renderer);
}

void GameEngine::RenderManager::Release()
{
	for (auto& renderer : m_Renderers)
	{
		delete renderer;
	}

	m_Renderers.clear();
}
