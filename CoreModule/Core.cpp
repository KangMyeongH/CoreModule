#include "Core.h"

GameEngine::Core::~Core()
{

}

void GameEngine::Core::Initialize(LPDIRECT3DDEVICE9 device)
{
	device->AddRef();

}

void GameEngine::Core::Progress()
{

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

	// RenderManager::Get_Instance->Render(m_Device);

	// render end
}
