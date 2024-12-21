#include "Core.h"
#include "RenderManager.h"

GameEngine::Core::~Core()
{

}

void GameEngine::Core::Initialize(LPDIRECT3DDEVICE9 device)
{
	m_Device = device;
	device->AddRef();

	//TODO : Manager ÃÊ±âÈ­
	m_RenderManager = &RenderManager::GetInstance();
}

void GameEngine::Core::Progress()
{
	//m_RenderManager->RegisterForUpdate();
}

void GameEngine::Core::Release()
{
	if (m_Device)
	{
		m_Device->Release();
	}
}

void GameEngine::Core::render(LPDIRECT3DDEVICE9 _device)
{
	// render begin
	m_RenderManager->Render_Begin(_device);

	//render
	m_RenderManager->Render(_device);

	// render end
	m_RenderManager->Render_End(_device);
}
