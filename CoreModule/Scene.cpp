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

bool GameEngine::Scene::Load_Scene(std::string sceneName)
{
	return false;
}

GameEngine::GameObject* GameEngine::Scene::Add_GameObject()
{
	GameObject* newGameObject = new GameObject();
	m_GameObjects.push_back(newGameObject);
	return newGameObject;
}

GameEngine::GameObjectList* GameEngine::Scene::Get_GameObjectList()
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

nlohmann::ordered_json GameEngine::Scene::To_Json() const
{
	nlohmann::ordered_json j = nlohmann::json
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

void GameEngine::Scene::From_Json(const nlohmann::ordered_json& _j)
{
	m_SceneName = _j.at("sceneName").get<std::string>();
	for (const auto& obj_json : _j.at("GameObjects"))
	{
		GameObject* obj = new GameObject();
		obj_json.get_to(*obj);
		m_GameObjects.push_back(obj);
	}
}

void GameEngine::to_json(nlohmann::ordered_json& _j, const Scene& _scene)
{
	_j = nlohmann::ordered_json
	{
	{"sceneName", _scene.m_SceneName},
	{"GameObjects", nlohmann::ordered_json::array()}
	};
	for (const auto& obj : _scene.m_GameObjects)
	{
		_j["GameObjects"].push_back(*obj);
	}
}

void GameEngine::from_json(const nlohmann::ordered_json& _j, Scene& _scene)
{
	_scene.m_SceneName = _j.at("sceneName").get<std::string>();
	for (const auto& obj_json : _j.at("GameObjects"))
	{
		GameObject* obj = new GameObject();
		obj_json.get_to(*obj);
		_scene.m_GameObjects.push_back(obj);
	}
}
