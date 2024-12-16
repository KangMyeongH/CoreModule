#include "Scene.h"

#include "GameObject.h"

IMPLEMENT_SINGLETON(GameEngine::Scene)

GameEngine::Scene::Scene()
{

}

GameEngine::Scene::~Scene()
{
}

bool GameEngine::Scene::Initialize()
{
	// Scene.json 파일을 읽어와서 GameObjectList에 GameObject들 넣어주기
	return true;
}

void GameEngine::Scene::Release()
{

}

bool GameEngine::Scene::LoadScene(std::string sceneName)
{
	return false;
}

GameEngine::GameObject* GameEngine::Scene::AddGameObject()
{

	return nullptr;
}

GameEngine::GameObject* GameEngine::Scene::Find(const std::string& _name)
{
	for(auto& go : m_GameObjects)
	{
		if (go->GetName() == _name)
		{
			return go;
		}
	}

	return nullptr;
}
