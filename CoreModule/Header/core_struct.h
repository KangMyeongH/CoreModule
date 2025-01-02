#pragma once

namespace GameEngine
{
	class Collider;

	//Texture Vertex 구조체
	struct VTXTEX
	{
		Vector3		Position;
		Vector3		Normal;
		Vector2		TextureUV;
	};
	const ULONG	FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;


	//Cube Vertex 구조체
	struct VTXCUBE
	{
		Vector3 Position;
		Vector3 Normal;
		D3DXCOLOR Color;
	};
	const ULONG FVF_COLOR = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE |  D3DFVF_TEX0;


	//Cube Index 구조체
	struct INDEX16
	{
		USHORT  _0;
		USHORT  _1;
		USHORT  _2;
	};

	//Ray Cast 구조체
	struct Ray
	{
		Vector3 Origin;			// Ray의 시작점	(월드 좌표)
		Vector3 Direction;		// Ray의 방향 	(정규화된 월드 벡터)

		Ray() : Origin(0,0,0), Direction(0,0,1) {}
		Ray(const Vector3& _origin, const Vector3& _direction) : Origin(_origin), Direction(_direction) {}

		Vector3 Get_Point(const float _distance) const
		{
			return Origin + Direction * _distance;
		}
	};

	struct RayHit
	{
		Vector3 Point;		// 충돌 지점
		Vector3 Normal;		// 충돌 법선 ( 지금은 미 구현 )
		float Distance;		// ray origin으로부터 거리. 즉, ray 시작점으로 부터 거리
		Collider* Collider; // 충돌된 collider

		RayHit() : Point(0,0,0), Normal(0,0,0), Distance(0.f), Collider(nullptr) {}
	};

	struct OBB
	{
		Vector3 Center;
		Vector3 AxisX;
		Vector3 AxisY;
		Vector3 AxisZ;
		Vector3 Extents;
	};
}

