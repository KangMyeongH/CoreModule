#pragma once
#include "core_define.h"

namespace GameEngine
{
	class Camera;

	class COREMODULE_API CameraManager
	{
	private:
		CameraManager() = default;
		~CameraManager();
	public:
		DECLARE_SINGLETON(CameraManager)

	public:
		void 		Update_Camera(LPDIRECT3DDEVICE9 _device9) const;
		void 		Set_CurrentCamera(Camera* _camera);
		Camera* 	Get_CurrentCamera() const { return m_CurrentCamera; }

	private:
		Camera* m_CurrentCamera;

	};

}

