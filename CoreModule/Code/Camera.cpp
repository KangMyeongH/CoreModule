#include "Camera.h"

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
	RenderManager::GetInstance().Set_ProjMat(projMat);
}

void GameEngine::Camera::Destroy()
{
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
