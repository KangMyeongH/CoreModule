#pragma once

namespace GameEngine
{
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

