#include "MonoBehaviour.h"

#include "MonoBehaviourManager.h"

void GameEngine::MonoBehaviour::Destroy()
{
	MonoBehaviourManager::GetInstance().Remove_MonoBehaviour(this);
}
