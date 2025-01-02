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
	// (1) 월드 행렬에서 R(3x3), t 추출
	// D3DXMATRIX가 row-major or column-major냐에 따라
	// _11,_12,_13 ... 의 위치가 달라질 수 있는데,
	// 일반적인 DirectX9 row-major 행렬에서
	// 
	// [ _11 _12 _13 _14 ]
	// [ _21 _22 _23 _24 ]
	// [ _31 _32 _33 _34 ]
	// [ _41 _42 _43 _44 ]
	//
	// 이라 가정하면,
	// 회전+스케일 부분 =
	// {
	//  	( _11,_12,_13 )
	//   	( _21,_22,_23 )
	//  	( _31,_32,_33 )
	//  }
	// 위치 = 	( _41,_42,_43 )
	D3DXMATRIX worldMat = Get_Transform().Get_WorldMatrix();

	// 로컬에서의 center, extents
	Vector3 localCenter = m_Center;
	Vector3 localExt = m_Size * 0.5f;

	// 1) 월드 공간에서의 center
	Vector3 worldCenter;
	{
		// center 변환
		D3DXVec3TransformCoord(&worldCenter, &localCenter, &worldMat);
	}

	// 2) 회전 행렬(또는 회전+스케일)에 의한 extents
	//    => 절댓값으로 누적
	// Rx = (worldMat._11, worldMat._21, worldMat._31) ...
	// 단, 여기선 각 행/열이 어떻게 배치되는지 주의해야 함
	//
	// DX9에서 _11,_12,_13 은 첫 행에 해당( x축 변환 성분 )
	// -> row0 = ( _11, _12, _13 )
	//    row1 = ( _21, _22, _23 )
	//    row2 = ( _31, _32, _33 )
	Vector3 row0(fabsf(worldMat._11), fabsf(worldMat._12), fabsf(worldMat._13));
	Vector3 row1(fabsf(worldMat._21), fabsf(worldMat._22), fabsf(worldMat._23));
	Vector3 row2(fabsf(worldMat._31), fabsf(worldMat._32), fabsf(worldMat._33));

	Vector3 newExtents;
	newExtents.x = row0.x * localExt.x + row0.y * localExt.y + row0.z * localExt.z;
	newExtents.y = row1.x * localExt.x + row1.y * localExt.y + row1.z * localExt.z;
	newExtents.z = row2.x * localExt.x + row2.y * localExt.y + row2.z * localExt.z;

	// 3) 최종 AABB = center ± newExtents
	Vector3 minVec = worldCenter - newExtents;
	Vector3 maxVec = worldCenter + newExtents;

	m_WorldMin = minVec;
	m_WorldMax = maxVec;

	/*
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
	m_WorldMax = maxVec;*/
}

bool GameEngine::BoxCollider::Check_OBBCollision(BoxCollider* _other, Vector3& _contactPoint)
{
	return false;
}

bool GameEngine::BoxCollider::Check_SphereCollision(SphereCollider* _other, Vector3& _contactPoint)
{
	return false;
}
