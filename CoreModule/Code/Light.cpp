#include "Light.h"

#include "Transform.h"

UINT GameEngine::Light::m_IndexValue = 0;

void GameEngine::Light::Ready_Light(LPDIRECT3DDEVICE9 _device)
{
	//클라이언트 실행 중에 조명 설정 변경하면 적용 안됨
	//Ready_Light는 조명 설정이 완료된 뒤에 실행되어야 함
	_device->SetLight(m_Index, m_LightInfo);
	_device->LightEnable(m_Index, TRUE); //해당 번호의 light가 조명 연산 수행하도록 설정
}

void GameEngine::Light::Update_Light(LPDIRECT3DDEVICE9 _device) const
{
	m_LightInfo->Position = Get_Transform().Position();

	Vector3 rotation = Get_Transform().Get_LocalRotation();
	D3DXMATRIX matRot;

	float yaw = D3DXToRadian(rotation.y);
	float pitch = D3DXToRadian(rotation.x);
	float roll = D3DXToRadian(rotation.z);

	D3DXQUATERNION quaternion;
	D3DXQuaternionRotationYawPitchRoll(&quaternion, yaw, pitch, roll);
	D3DXQuaternionNormalize(&quaternion, &quaternion);
	D3DXMatrixRotationQuaternion(&matRot, &quaternion);

	Vector3 direction{ matRot._31, matRot._32, matRot._33 };
	D3DXVec3Normalize(&direction, &direction);
	m_LightInfo->Direction = direction;
}

void GameEngine::Light::Destroy()
{
}

void GameEngine::Light::to_json(nlohmann::ordered_json& _j)
{
	std::string type = "Light";
	_j = nlohmann::ordered_json{
		{"type", type}
	};
}

void GameEngine::Light::from_json(const nlohmann::ordered_json& _j)
{
}

GameEngine::Component* GameEngine::Light::Clone() const
{
	return new Light(*this);
}
