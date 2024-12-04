#pragma once
#include <list>
#include <string>
// 직렬화,,,, 역직렬화,,,, 어캐 하냐 << 끗
// Serialize 관련 매니저를 하나 둬야하나? << 둬야함
// GameObject를 이곳 저곳에 저장하는건 메모리 낭비 같은데 << 맞음 그래서 그전에 짠 프레임 워크가 매우 비효율적
// 그러면 Scene이 하는 역할? 저장하는거? << 데이터들 가지고 있는거
// (런타임 기준) Scene은 GameObject들을 가지고 있고, GameObject는 연결된 Component들을 가지고 있는다.
// (Json 파일 기준) Scene.Json이 저장하는 정보들
// 1. GameObject 계층 구조 (Hierarchy)
// 2. GameObject에 연결된 모든 Component의 데이터가 저장됨.
// 3. Scene 클래스가 과연 필요할까?
// 4. 로딩까지 생각해서?

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
