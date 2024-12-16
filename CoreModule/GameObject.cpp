#include "GameObject.h"

GameEngine::GameObject::GameObject(): m_Transform(this), m_bActiveSelf(true), m_bStatic(false)
{
}

GameEngine::GameObject::~GameObject()
{
}

void GameEngine::GameObject::Destroy()
{
}
