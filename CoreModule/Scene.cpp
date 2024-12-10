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
	// Scene.json ������ �о�ͼ� GameObjectList�� GameObject�� �־��ֱ�
	return true;
}

void GameEngine::Scene::Release()
{

}

bool GameEngine::Scene::LoadScene(std::string sceneName)
{
	return false;
}

std::weak_ptr<GameEngine::GameObject> GameEngine::Scene::AddGameObject()
{

	return {};
}

std::weak_ptr<GameEngine::GameObject> GameEngine::Scene::Find(const std::string& name)
{
	for(auto& go : mGameObjects)
	{
		if (go->GetName() == name)
		{
			return go;
		}
	}

	return {};
}
