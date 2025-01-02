#pragma once

namespace GameEngine
{
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

