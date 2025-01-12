#include "UIManager.h"

void GameEngine::UIManager::Register_UI()
{
}

void GameEngine::UIManager::Render_UI()
{
}

void GameEngine::UIManager::Add_TextureUI(TextureUI* _textureUI)
{
	m_RegisterQueue.push_back(_textureUI);
}
