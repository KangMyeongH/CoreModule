#include "UI.h"

#include "InputManager.h"
#include "Transform.h"
#include "UIManager.h"

GameEngine::UI::~UI() = default;

void GameEngine::UI::Destroy()
{
	UIManager::GetInstance().Remove_Renderer(this);
}
