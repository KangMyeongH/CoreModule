#include "CubeRenderer.h"

#include "GameObject.h"
#include "Transform.h"

void GameEngine::CubeRenderer::Render(LPDIRECT3DDEVICE9 _device)
{
	D3DXMATRIX matWorld = Get_Transform().Get_WorldMatrix();
}
