#include "Scene.h"

#include <fstream>

#include "GameObject.h"
#include "Camera.h"
#include "CameraManager.h"
#include "FileManager.h"
#include "TimeManager.h"

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

void GameEngine::Scene::Change_Scene(const std::wstring& _sceneName)
{
	m_NextScene = _sceneName;
}

GameEngine::GameObject* GameEngine::Scene::Add_GameObject()
{
	GameObject* newGameObject = new GameObject();
	m_GameObjects.push_back(newGameObject);
	return newGameObject;
}

GameEngine::GameObject* GameEngine::Scene::Add_GameObject(const std::string& _name)
{
	GameObject* newGameObject = new GameObject();
	m_GameObjects.push_back(newGameObject);
	newGameObject->Set_Name(_name);
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

void GameEngine::Scene::Register_NextScene()
{
	if (!m_NextScene.empty())
	{
		if (!UIManager::GetInstance().Is_FadeOut())
		{
			UIManager::GetInstance().Set_FadeEffect(1.f);
			UIManager::GetInstance().Enable_FadeOut();
		}

		if (UIManager::GetInstance().Is_FadeOutFinish())
		{
			if (!m_LoadingThread.joinable())
			{
				// 교체할 씬의 Path 조합
				const std::wstring basePath = L"..\\Client\\Assets\\Scenes\\";
				const std::wstring fileExtension = L".json";
				const std::wstring fullPath = basePath + m_NextScene + fileExtension;

				m_bSceneLoaded = false;
				m_LoadingThread = std::thread(&GameEngine::Scene::Load_SceneInBackGround, this, fullPath);
			}

			while (true)
			{
				MSG msg;
				while (PeekMessage(&msg, nullptr, 0,0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				UIManager::GetInstance().Render_LoadingScreen();

				{
					std::unique_lock<std::mutex> lock(m_LoadingMutex);
					if (m_bSceneLoaded)
					{
						break;
					}
				}

				Sleep(16);
			}
			TimeManager::GetInstance().Initialize();
					// 다 불러왔으니 NextScene의 path 삭제
			m_NextScene.clear();
					// 로딩이 끝났으니 페이드 인
			UIManager::GetInstance().Enable_FadeIn();

			if (m_LoadingThread.joinable())
			{
				m_LoadingThread.join();
			}
		}
	}
}

void GameEngine::Scene::Destroy_GameObject()
{
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end();)
	{
		if ((*it)->Is_Destroyed())
		{
			delete* it;
			it = m_GameObjects.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void GameEngine::Scene::Register_Component()
{
	GameObjectList gameObjects = m_GameObjects;
	for (auto& gameObject : gameObjects)
	{
		Component_Map copy = gameObject->Get_ComponentMap();
		for (auto& componentVec : copy)
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

				if (dynamic_cast<Collider*>(component))
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

				if (dynamic_cast<UI*>(component))
				{
					UIManager::GetInstance().Add_UI(dynamic_cast<UI*>(component));
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

void GameEngine::Scene::Load_SceneInBackGround(const std::wstring& _nextScenePath)
{
	Release();
	MonoBehaviourManager::GetInstance().Release();
	CollisionManager::GetInstance().Release();
	PhysicsManager::GetInstance().Release();
	RenderManager::GetInstance().Clear_Component();
	UIManager::GetInstance().Clear_Component();

	FileManager::GetInstance().LoadSceneData(_nextScenePath);

	PhysicsManager::GetInstance().Register_Rigidbody();
	CollisionManager::GetInstance().Register_Collider();
	RenderManager::GetInstance().Register_Renderer();
	UIManager::GetInstance().Register_UI();
	MonoBehaviourManager::GetInstance().Register_MonoBehaviour();

	{
		std::lock_guard<std::mutex> lock(m_LoadingMutex);
		m_bSceneLoaded = true;
	}

	m_CV.notify_one();
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
