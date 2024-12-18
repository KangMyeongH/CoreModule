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
