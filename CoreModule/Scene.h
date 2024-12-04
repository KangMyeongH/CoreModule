#pragma once
#include <list>
#include <string>
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
}

namespace GameEngine
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		bool Initialize();

	private:
		std::string 				mName;
		std::list<GameObject*> 		mGameObjects;
	};

}
