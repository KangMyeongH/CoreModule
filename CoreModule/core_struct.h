#pragma once

namespace GameEngine
{
	//Cube Vertex 구조체
	struct VTXCUBE
	{
		Vector3 Position;
		Vector3 TextureUV;
	};
	const ULONG FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	//Cube Index 구조체
	struct INDEX16
	{
		USHORT  _0;
		USHORT  _1;
		USHORT  _2;

	};
}

