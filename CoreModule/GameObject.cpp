#include "GameObject.h"

GameEngine::GameObject::GameObject() : mTransform(std::shared_ptr<GameObject>(this)),mTag("Untagged"), mActiveSelf(true), mbStatic(false)
{
}

GameEngine::GameObject::~GameObject()
{
}
