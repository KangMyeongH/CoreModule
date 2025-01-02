#pragma once

namespace GameEngine
{
	class Collider;

	//Texture Vertex ����ü
	struct VTXTEX
	{
		Vector3		Position;
		Vector3		Normal;
		Vector2		TextureUV;
	};
	const ULONG	FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;


	//Cube Vertex ����ü
	struct VTXCUBE
	{
		Vector3 Position;
		Vector3 Normal;
		D3DXCOLOR Color;
	};
	const ULONG FVF_COLOR = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE |  D3DFVF_TEX0;


	//Cube Index ����ü
	struct INDEX16
	{
		USHORT  _0;
		USHORT  _1;
		USHORT  _2;
	};

	//Ray Cast ����ü
	struct Ray
	{
		Vector3 Origin;			// Ray�� ������	(���� ��ǥ)
		Vector3 Direction;		// Ray�� ���� 	(����ȭ�� ���� ����)

		Ray() : Origin(0,0,0), Direction(0,0,1) {}
		Ray(const Vector3& _origin, const Vector3& _direction) : Origin(_origin), Direction(_direction) {}

		Vector3 Get_Point(const float _distance) const
		{
			return Origin + Direction * _distance;
		}
	};

	struct RayHit
	{
		Vector3 Point;		// �浹 ����
		Vector3 Normal;		// �浹 ���� ( ������ �� ���� )
		float Distance;		// ray origin���κ��� �Ÿ�. ��, ray ���������� ���� �Ÿ�
		Collider* Collider; // �浹�� collider

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

