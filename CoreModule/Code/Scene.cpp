#include "Scene.h"

#include <fstream>

#include "GameObject.h"
#include "Camera.h"
#include "CameraManager.h"

IMPLEMENT_SINGLETON(GameEngine::Scene)

GameEngine::Scene::Scene() : m_SceneName("SampleScene")
{

}

GameEngine::Scene::~Scene()
{
	Release();
}

bool GameEngine::Scene::Initialize(const std::wstring& _path)
{
	if (_path.empty()) return false;
	std::ifstream inFile(_path);
	if (!inFile.is_open())
	{
		return false;
	}
	nlohmann::ordered_json j;
	inFile >> j;
	From_Json(j);
	inFile.clear();
	inFile.close();

	return false;
}

void GameEngine::Scene::Release()
{
	for (auto& obj : m_GameObjects)
	{
		delete obj;
	}
	m_GameObjects.clear();
}

bool GameEngine::Scene::Load_Scene(std::string _sceneName)
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

void GameEngine::Scene::Destroy_GameObject()
{
	for (auto& gameObject : m_GameObjects)
	{
		if (gameObject->Is_Destroyed())
		{
			delete gameObject;
			m_GameObjects.erase(std::remove(m_GameObjects.begin(), m_GameObjects.end(), gameObject), m_GameObjects.end());
		}
	}
}

void GameEngine::Scene::Register_Component()
{
	for (auto& gameObject : m_GameObjects)
	{
		for (auto& componentVec : gameObject->Get_ComponentMap())
		{
			for (auto& component : componentVec.second)
			{
				if (dynamic_cast<MonoBehaviour*>(component))
				{
					MonoBehaviourManager::GetInstance().Add_MonoBehaviour(dynamic_cast<MonoBehaviour*>(component));
					continue;
				}

				if (dynamic_cast<Rigidbody*>(component))
				{
					PhysicsManager::GetInstance().Add_Rigidbody(dynamic_cast<Rigidbody*>(component));
					continue;
				}

				if (dynamic_cast<BoxCollider*>(component))
				{
					CollisionManager::GetInstance().Add_Collider(dynamic_cast<Collider*>(component));
					continue;
				}

				if (dynamic_cast<Renderer*>(component))
				{
					RenderManager::GetInstance().Add_Renderer(dynamic_cast<Renderer*>(component));
					continue;
				}

				if (dynamic_cast<Light*>(component))
				{
					RenderManager::GetInstance().Add_Light(dynamic_cast<Light*>(component));
					continue;
				}

				if (dynamic_cast<Camera*>(component))
				{
					CameraManager::GetInstance().Set_CurrentCamera(dynamic_cast<Camera*>(component));
					continue;
				}
			}
		}
	}
}

nlohmann::ordered_json GameEngine::Scene::To_Json() const
{
	nlohmann::ordered_json j = nlohmann::ordered_json
	{
		{"sceneName", m_SceneName},
		{"GameObjects", nlohmann::ordered_json::array()}
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
	Register_Component();
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
