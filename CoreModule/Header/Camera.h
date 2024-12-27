#pragma once
#include "Behaviour.h"

namespace GameEngine
{
	class COREMODULE_API Camera : public Behaviour
	{
	public:
		Camera() : Behaviour(nullptr),
			m_Target(0.0f, 0.0f, -10.f),
			m_UP(0.0f, 1.0f, 0.0f),
			m_FiledOfView(45.f),
			m_AspectRatio(1.6f),
			m_NearPlane(0.3f),
			m_FarPlane(1000.0f)
		{}

		explicit Camera(GameObject* _owner) : Behaviour(_owner),
			m_Target(0.0f, 0.0f, -10.f),
			m_UP(0.0f, 1.0f, 0.0f),
			m_FiledOfView(45.f),
			m_AspectRatio(1.6f),
			m_NearPlane(0.3f),
			m_FarPlane(1000.0f)
		{}

		void 	Update_Camera(LPDIRECT3DDEVICE9 _device);

		void 	Set_AspectRatio(const float _aspectRatio) { m_AspectRatio = _aspectRatio; }

		void 	Set_FOV(const float _fov) { m_FiledOfView = _fov; }
		float 	Get_FOV() const { return m_FiledOfView; }

		void	Set_NearPlane(const float _near) { m_NearPlane = _near; }
		float	Get_NearPlane() const { return m_NearPlane; }

		void	Set_FarPlane(const float _far) { m_FarPlane = _far; }
		float	Get_FatPlane() const { return m_FarPlane; }
		void 	Destroy() override;
		void 	to_json(nlohmann::ordered_json& _j) override;
		void 	from_json(const nlohmann::ordered_json& _j) override;

		Component* Clone() const override;

	private:
		Vector3 m_Target;
		Vector3 m_UP;

		float m_FiledOfView;
		float m_AspectRatio;
		float m_NearPlane;
		float m_FarPlane;
	};

	REGISTER_COMPONENT(Camera)
}

