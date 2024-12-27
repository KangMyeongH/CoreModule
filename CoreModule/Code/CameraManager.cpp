#include "CameraManager.h"

#include "Camera.h"

GameEngine::CameraManager::~CameraManager()
{
	if (m_CurrentCamera != nullptr)
	{
		delete m_CurrentCamera;
	}
}

IMPLEMENT_SINGLETON(GameEngine::CameraManager)

void GameEngine::CameraManager::Update_Camera(LPDIRECT3DDEVICE9 _device9) const
{
	m_CurrentCamera->Update_Camera(_device9);
}

void GameEngine::CameraManager::Set_CurrentCamera(Camera* _camera)
{
	m_CurrentCamera = _camera;
}
