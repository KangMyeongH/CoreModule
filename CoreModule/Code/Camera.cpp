#include "Camera.h"

#include "CameraManager.h"
#include "RenderManager.h"
#include "Transform.h"

void GameEngine::Camera::Update_Camera(LPDIRECT3DDEVICE9 _device)
{
	Vector3 rotation = Get_Transform().Get_LocalRotation();
	D3DXMATRIX matRot;

	float yaw = D3DXToRadian(rotation.y);
	float pitch = D3DXToRadian(rotation.x);
	float roll = D3DXToRadian(rotation.z);

	D3DXQUATERNION quaternion;
	D3DXQuaternionRotationYawPitchRoll(&quaternion, yaw, pitch, roll);
	D3DXQuaternionNormalize(&quaternion, &quaternion);
	D3DXMatrixRotationQuaternion(&matRot, &quaternion);

	m_Target = Get_Transform().Get_LocalPosition() + Vector3(matRot._31, matRot._32, matRot._33);

	D3DXMATRIX viewMat, projMat;

	float fov = D3DXToRadian(m_FiledOfView);
	D3DXMatrixLookAtLH(&viewMat, &Get_Transform().Get_LocalPosition(), &m_Target, &m_UP);
	D3DXMatrixPerspectiveFovLH(&projMat, fov, m_AspectRatio, m_NearPlane, m_FarPlane);

	RenderManager::GetInstance().Set_ViewMat(viewMat);
	m_ViewMat = viewMat;
	RenderManager::GetInstance().Set_ProjMat(projMat);
	m_ProjMat = projMat;
}

GameEngine::Ray GameEngine::Camera::ScreenPointToRay(const Vector3& _position)
{
	D3DVIEWPORT9 vp;
	RenderManager::GetInstance().Get_Device()->GetViewport(&vp);

	Vector3 nearPoint(_position.x, _position.y, 0.0f);
	Vector3 farPoint(_position.x, _position.y, 1.0f);

	Vector3 nearPointWorld, farPointWorld;
	D3DXMATRIX identityWorld;
	D3DXMatrixIdentity(&identityWorld);
	D3DXVec3Unproject(
		&nearPointWorld,
		&nearPoint,
		&vp,
		&m_ProjMat,
		&m_ViewMat,
		&identityWorld
	);
	D3DXVec3Unproject(
		&farPointWorld,
		&farPoint,
		&vp,
		&m_ProjMat,
		&m_ViewMat,
		&identityWorld
	);

	Ray ray;
	ray.Origin = nearPointWorld;
	Vector3 dir = farPointWorld - nearPointWorld;
	dir.Normalize();
	ray.Direction = dir;

	return ray;
}

void GameEngine::Camera::Destroy()
{
	if (CameraManager::GetInstance().Get_CurrentCamera() == this)
	{
		CameraManager::GetInstance().Set_CurrentCamera(nullptr);

		// 이게맞나
		delete this;
	}
}

void GameEngine::Camera::to_json(nlohmann::ordered_json& _j)
{
	std::string type = "Camera";
	_j = nlohmann::ordered_json{
		{"type", type}
	};
}

void GameEngine::Camera::from_json(const nlohmann::ordered_json& _j)
{

}

GameEngine::Component* GameEngine::Camera::Clone() const
{
	return new Camera(*this);
}
