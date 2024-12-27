#include "Core.h"

#include "MonoBehaviourManager.h"
#include "PhysicsManager.h"
#include "RenderManager.h"
#include "TimeManager.h"
#include "CameraManager.h"
#include "Scene.h"

GameEngine::Core::~Core()
{

}

void GameEngine::Core::Initialize(LPDIRECT3DDEVICE9 device)
{
	m_Device = device;
	device->AddRef();

	m_Scene = &Scene::GetInstance();

	//TODO : Manager ÃÊ±âÈ­
	m_TimeManager = &TimeManager::GetInstance();
	m_TimeManager->Initialize();
	m_CameraManager = &CameraManager::GetInstance();
	m_RenderManager = &RenderManager::GetInstance();
	m_PhysicsManager = &PhysicsManager::GetInstance();
	m_MonoBehaviourManager = &MonoBehaviourManager::GetInstance();
}

void GameEngine::Core::Progress()
{
	//m_RenderManager->RegisterForUpdate();
	m_TimeManager->TimeUpdate();
	m_PhysicsManager->Register_Rigidbody();
	m_RenderManager->Register_Renderer();

	m_MonoBehaviourManager->Register_MonoBehaviour();

	start();
	fixed_Update();
	m_PhysicsManager->Physics_Update(m_TimeManager->Get_DeltaTime());
	on_Trigger();
	on_Collision();
	update();
	late_Update();
	render(m_Device);
	on_Destroy();
	destroy();
}

void GameEngine::Core::EditorProgress()
{
	m_TimeManager->TimeUpdate();
	m_PhysicsManager->Register_Rigidbody();
	m_RenderManager->Register_Renderer();

	m_MonoBehaviourManager->Register_MonoBehaviour();

	start();
	fixed_Update();
	m_PhysicsManager->Physics_Update(m_TimeManager->Get_DeltaTime());
	on_Trigger();
	on_Collision();
	update();
	late_Update();
}

void GameEngine::Core::Decommissioning()
{
	on_Destroy();
	destroy();
	m_TimeManager->FrameLimit();
}

void GameEngine::Core::Release()
{
	if (m_Device)
	{
		m_Device->Release();
	}
}

void GameEngine::Core::start()
{
	m_MonoBehaviourManager->Register_MonoBehaviour();
}

void GameEngine::Core::fixed_Update()
{
	m_MonoBehaviourManager->Fixed_Update();
}

void GameEngine::Core::on_Trigger()
{
}

void GameEngine::Core::on_Collision()
{
}

void GameEngine::Core::update()
{
	m_MonoBehaviourManager->Update();
}

void GameEngine::Core::late_Update()
{
	m_MonoBehaviourManager->Late_Update();
}

void GameEngine::Core::on_Destroy()
{
	m_MonoBehaviourManager->On_Destroy();
}

void GameEngine::Core::destroy()
{
	m_Scene->Destroy_GameObject();
	m_PhysicsManager->Destroy_Rigidbody();
	m_RenderManager->Destroy_Renderer();
	m_MonoBehaviourManager->Destroy_MonoBehaviour();
}

void GameEngine::Core::render(LPDIRECT3DDEVICE9 _device)
{
	m_CameraManager->Update_Camera(_device);

	// render begin
	m_RenderManager->Render_Begin(_device);

	//render
	m_RenderManager->Render(_device);

	// render end
	m_RenderManager->Render_End(_device);
}
