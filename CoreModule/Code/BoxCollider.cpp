#include "BoxCollider.h"
#include "SphereCollider.h"
#include "Transform.h"

void GameEngine::BoxCollider::Update_Collider()
{
	Calc_WorldAABB();
	Calc_WorldOBB();
}

bool GameEngine::BoxCollider::Check_Collision(Collider* _other, Vector3& _contactPoint)
{
	if (_other->Get_ColliderType() == ColliderType::Box)
	{
		return Check_OBBCollision(dynamic_cast<BoxCollider*>(_other), _contactPoint);
	}

	if (_other->Get_ColliderType() == ColliderType::Sphere)
	{
		return Check_SphereCollision(dynamic_cast<SphereCollider*>(_other), _contactPoint);
	}

	return false;
}

GameEngine::OBB GameEngine::BoxCollider::Calc_WorldOBB()
{
	OBB obb;

	Vector3 axes[3];

	D3DXMATRIX worldMat = Get_Transform().Get_WorldMatrix();

	// 축 벡터 추출 (월드행렬의 X, Y, Z컬럼)
	Vector3 colX(worldMat._11, worldMat._21, worldMat._31);
	Vector3 colY(worldMat._12, worldMat._22, worldMat._32);
	Vector3 colZ(worldMat._13, worldMat._23, worldMat._33);

	// 각 컬럼 길이가 scale
	float sx = D3DXVec3Length(&colX);
	float sy = D3DXVec3Length(&colY);
	float sz = D3DXVec3Length(&colZ);

	Vector3 scale = Vector3(sx, sy, sz);

	// 축 벡터는 정규화
	if (sx > 1e-6f) colX /= sx;
	if (sy > 1e-6f) colY /= sy;
	if (sz > 1e-6f) colZ /= sz;

	obb.AxisX = colX;
	obb.AxisY = colY;
	obb.AxisZ = colZ;

	// Center 월드 변환
	D3DXVECTOR4 transformedCenter;
	D3DXVec3Transform(&transformedCenter, &m_Center, &worldMat);
	obb.Center = Vector3(transformedCenter.x, transformedCenter.y, transformedCenter.z);

	obb.Extents = Vector3{
		m_Size.x * scale.x * 0.5f,
		m_Size.y * scale.y * 0.5f,
		m_Size.z * scale.z * 0.5f };

	m_OBB = obb;

	return obb;
}

void GameEngine::BoxCollider::Calc_WorldAABB()
{
	D3DXMATRIX worldMat = Get_Transform().Get_WorldMatrix();

	Vector3 extents = m_Size * 0.5f;

	Vector3 corners[8] = {
		Vector3(-extents.x, -extents.y, -extents.z),
		Vector3( extents.x, -extents.y, -extents.z),
		Vector3(-extents.x,  extents.y, -extents.z),
		Vector3( extents.x,  extents.y, -extents.z),
		Vector3(-extents.x, -extents.y,  extents.z),
		Vector3( extents.x, -extents.y,  extents.z),
		Vector3(-extents.x,  extents.y,  extents.z),
		Vector3( extents.x,  extents.y,  extents.z),
	};

	for (auto& corner : corners)
	{
		corner += m_Center;
	}

	Vector3 minVec( FLT_MAX,  FLT_MAX,  FLT_MAX);
	Vector3 maxVec(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (auto& corner : corners)
	{
		Vector3 worldPos;
		D3DXVec3TransformCoord(&worldPos, &corner, &worldMat);

		// Min / Max 갱신
		minVec.x = min(minVec.x, worldPos.x);
		minVec.y = min(minVec.y, worldPos.y);
		minVec.z = min(minVec.z, worldPos.z);

		maxVec.x = max(maxVec.x, worldPos.x);
		maxVec.y = max(maxVec.y, worldPos.y);
		maxVec.z = max(maxVec.z, worldPos.z);
	}

	m_WorldMin = minVec;
	m_WorldMax = maxVec;
}

bool GameEngine::BoxCollider::Check_OBBCollision(BoxCollider* _other, Vector3& _contactPoint)
{
	return false;
}

bool GameEngine::BoxCollider::Check_SphereCollision(SphereCollider* _other, Vector3& _contactPoint)
{
	return false;
}
