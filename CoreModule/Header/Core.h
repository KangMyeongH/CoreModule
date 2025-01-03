#pragma once
#include <d3d9.h>

#include "core_define.h"

namespace GameEngine
{
	class InputManager;
	class Scene;
}

namespace GameEngine
{
	class CameraManager;
}

namespace GameEngine
{
	class PhysicsManager;
	class TimeManager;
	class RenderManager;
	class MonoBehaviourManager;

	class COREMODULE_API Core
	{
	private:
		Core() = default;
		~Core();
	public:
		Core(const Core&) = delete;
		Core(Core&&) = delete;
		Core& operator=(const Core&) = delete;
		Core& operator=(Core&&) = delete;

		static Core& Get_Instance() { static Core s_Core; return s_Core; }

	public:
		void Initialize(LPDIRECT3DDEVICE9 device);
		inline void Progress();
		inline void EditorProgress();
		inline void Decommissioning();
		void Release();

	private:
		void start();
		void fixed_Update();
		void on_Trigger();
		void on_Collision();
		void update();
		void late_Update();
		void render(LPDIRECT3DDEVICE9 _device);
		void on_Destroy();
		void destroy();

	private:
		LPDIRECT3DDEVICE9 		m_Device;

		Scene* 					m_Scene;

		InputManager* 			m_InputManager;
		PhysicsManager* 		m_PhysicsManager;
		RenderManager* 			m_RenderManager;
		MonoBehaviourManager*	m_MonoBehaviourManager;
		TimeManager* 			m_TimeManager;
		CameraManager* 			m_CameraManager;
	};

}

