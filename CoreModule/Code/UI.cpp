#include "UI.h"

#include "UIManager.h"

GameEngine::UI::~UI() = default;

void GameEngine::UI::Destroy()
{
	UIManager::GetInstance().Remove_Renderer(this);
}