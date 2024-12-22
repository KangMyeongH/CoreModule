#pragma once

namespace GameEngine
{
	//Texture Vertex 구조체
	struct VTXTEX
	{
		Vector3		Position;
		Vector3		Normal;
		Vector3		TextureUV;

	};
	const ULONG	FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;


	//Cube Vertex 구조체
	struct VTXCUBE
	{
		Vector3 Position;
		ULONG	Color;
	};
	const ULONG FVF_COLOR = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;


	//Cube Index 구조체
	struct INDEX16
	{
		USHORT  _0;
		USHORT  _1;
		USHORT  _2;

	};
}

