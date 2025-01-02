#include "Collider.h"

#include "../CollisionManager.h"

void GameEngine::Collider::Destroy()
{
	CollisionManager::GetInstance().Remove_Collider(this);
}
