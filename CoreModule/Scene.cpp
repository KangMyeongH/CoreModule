#include "Scene.h"

#include "GameObject.h"

IMPLEMENT_SINGLETON(GameEngine::Scene)

GameEngine::Scene::Scene() : m_SceneName("SampleScene")
{

}

GameEngine::Scene::~Scene()
{
	for (auto& obj : m_GameObjects)
	{
		delete obj;
	}
}

bool GameEngine::Scene::Initialize(const std::string& name, const GameObjectList& gameObjects)
{
	// Scene.json 파일을 읽어와서 GameObjectList에 GameObject들 넣어주기
	m_SceneName = name;
	m_GameObjects = gameObjects;

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
	GameObject* newGameObject = new GameObject();
	m_GameObjects.push_back(newGameObject);
	return newGameObject;
}

GameEngine::GameObjectList* GameEngine::Scene::GetGameObjectList()
{
	return &m_GameObjects;
}

GameEngine::GameObject* GameEngine::Scene::Find(const std::string& _name)
{
	for(auto& go : m_GameObjects)
	{
		if (go->Get_Name() == _name)
		{
			return go;
		}
	}

	return nullptr;
}

nlohmann::json GameEngine::Scene::ToJson() const
{
	nlohmann::json j;
	j = nlohmann::json
	{
	{"sceneName", m_SceneName},
	{"GameObjects", nlohmann::json::array()}
	};
	for (const auto& obj : m_GameObjects)
	{
		j["GameObjects"].push_back(*obj);
	}

	return j;
}

void GameEngine::Scene::FromJson(const nlohmann::json& j)
{
	m_SceneName = j.at("sceneName").get<std::string>();
	for (const auto& obj_json : j.at("GameObjects"))
	{
		GameObject* obj = new GameObject();
		obj_json.get_to(*obj);
		m_GameObjects.push_back(obj);
	}
}

void GameEngine::to_json(nlohmann::json& j, const Scene& scene)
{
	j = nlohmann::json
	{
	{"sceneName", scene.m_SceneName},
	{"GameObjects", nlohmann::json::array()}
	};
	for (const auto& obj : scene.m_GameObjects)
	{
		j["GameObjects"].push_back(*obj);
	}
}

void GameEngine::from_json(const nlohmann::json& j, Scene& scene)
{
	scene.m_SceneName = j.at("sceneName").get<std::string>();
	for (const auto& obj_json : j.at("GameObjects"))
	{
		GameObject* obj = new GameObject();
		obj_json.get_to(*obj);
		scene.m_GameObjects.push_back(obj);
	}
}
