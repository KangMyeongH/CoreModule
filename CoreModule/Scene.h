#pragma once
#include <list>
#include <memory>
#include <string>

#include "core_define.h"
// ����ȭ,,,, ������ȭ,,,, ��ĳ �ϳ� << ��
// Serialize ���� �Ŵ����� �ϳ� �־��ϳ�? << �־���
// GameObject�� �̰� ������ �����ϴ°� �޸� ���� ������ << ���� �׷��� ������ § ������ ��ũ�� �ſ� ��ȿ����
// �׷��� Scene�� �ϴ� ����? �����ϴ°�? << �����͵� ������ �ִ°�
// (��Ÿ�� ����) Scene�� GameObject���� ������ �ְ�, GameObject�� ����� Component���� ������ �ִ´�.
// (Json ���� ����) Scene.Json�� �����ϴ� ������
// 1. GameObject ���� ���� (Hierarchy)
// 2. GameObject�� ����� ��� Component�� �����Ͱ� �����.
// 3. Scene Ŭ������ ���� �ʿ��ұ�?
// 4. �ε����� �����ؼ�?

namespace GameEngine
{
	class GameObject;

	using GameObjectList = std::list<GameObject*>;

	class COREMODULE_API Scene
	{
	private:
		//======================================//
		//				constructor				//
		//======================================//
		Scene();
		~Scene();
	public:
		DECLARE_SINGLETON(Scene)

	public:
		//======================================//
		//				  method				//
		//======================================//
		// Scene management
		bool Initialize(const std::string& name, const GameObjectList& gameObjects);
		void Release();
		bool LoadScene(std::string sceneName);

		// GameObject management
		GameObject* AddGameObject();
		GameObjectList* GetGameObjectList();
		GameObject* Find(const std::string& _name);

		std::string GetSceneName() { return m_SceneName; }

		nlohmann::json ToJson() const;
		void FromJson(const nlohmann::json& j);

		friend void to_json(nlohmann::json& j, const Scene& scene);
		friend void from_json(const nlohmann::json& j, Scene& scene);
		
	private:
		GameObjectList	m_GameObjects;
		std::string 	m_SceneName;
	};

}
