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
		bool Initialize();
		void Release();
		bool LoadScene(std::string sceneName);

		// GameObject management
		GameObject* AddGameObject();
		GameObject* Find(const std::string& _name);
		
	private:
		GameObjectList	m_GameObjects;
		std::string 	m_Name;
	};

}
