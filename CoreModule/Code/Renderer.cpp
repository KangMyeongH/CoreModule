#include "Renderer.h"

#include "RenderManager.h"

void GameEngine::Renderer::Destroy()
{
	RenderManager::GetInstance().Remove_Renderer(this);
}
