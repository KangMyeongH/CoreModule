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
		bool Initialize(const std::wstring& _path);
		void Release();
		bool Load_Scene(std::string _sceneName);

		// GameObject management
		GameObject* 		Add_GameObject();
		GameObjectList* 	Get_GameObjectList();
		GameObject* 		Find(const std::string& _name);

		std::string Get_SceneName() { return m_SceneName; }

		void Destroy_GameObject();

		void Register_Component();

		nlohmann::ordered_json To_Json() const;
		void From_Json(const nlohmann::ordered_json& _j);

		friend void to_json(nlohmann::ordered_json& _j, const Scene& _scene);
		friend void from_json(const nlohmann::ordered_json& _j, Scene& _scene);
		
	private:
		GameObjectList			m_GameObjects;
		std::string 			m_SceneName;
	};

}
