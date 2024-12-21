#pragma once

namespace GameEngine
{
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

