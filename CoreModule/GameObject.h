#pragma once
#include <typeindex>
#include <unordered_map>

#include "Component.h"
#include "Object.h"
#include "Scene.h"
#include "Transform.h"


// 게임 오브젝트의 생성을 호출 즉시 바로
// 게임 오브젝트의 삭제는 삭제 호출 시 비활성화 후 프레임의 끝에서 삭제해줌
// 게임 오브젝트는 Transform Component를 반드시 소유함.


namespace GameEngine
{
	class Component;

	using Component_Map = std::unordered_map<std::type_index, std::vector<std::weak_ptr<Component>>>;

	class COREMODULE_API GameObject final : public Object
	{
	public:
		//======================================//
		//				constructor				//
		//======================================//

		GameObject();
		~GameObject() override;
		GameObject(const GameObject& rhs) : Object(rhs), mTransform(rhs.mTransform)
		{
			mTransform.SetOwner(std::make_shared<GameObject>(*this));

			for (const auto& pair : rhs.mComponentMap)
			{
				const auto& type = pair.first;
				const auto& components = pair.second;
				std::vector<std::weak_ptr<Component>> clonedComponents;
				for (const auto& componentWeak : components)
				{
					if (auto component = componentWeak.lock())
					{
						component->SetOwner(std::make_shared<GameObject>(*this));
						auto clonedComponent = std::shared_ptr<Component>(component);
						clonedComponents.push_back(clonedComponent);
					}
				}
				mComponentMap[type] = std::move(clonedComponents);
			}
		}

	public:
		//======================================//
		//				  method				//
		//======================================//

		// Component management
		template <typename T, typename... Args>
		std::weak_ptr<T> 				AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
			std::shared_ptr<T> component = std::make_shared<T>(this, std::forward<Args>(args)...);
			mComponentMap[typeid(T)].push_back(component);
			// 각각의 Component매니저에 주소값 뿌리기.

			return component;
		}
		template <typename T>
		std::weak_ptr<T> 				GetComponent()
		{
			auto it = mComponentMap.find(typeid(T));
			if (it != mComponentMap.end() && !it->second.empty())
			{
				return &it->second;
			}

			return nullptr;
		}
		template <typename T>
		std::vector<std::weak_ptr<T>>* 	GetComponents() const
		{
			auto it = mComponentMap.find(typeid(T));
			if (it != mComponentMap.end() && !it->second.empty())
			{
				return &it->second;
			}
			return nullptr;
		}
		Transform& 						GetTransform() { return mTransform; }
		

		//======================================//
		//			   static method			//
		//======================================//
		// 이름을 사용해서 현재 씬의 GameObject를 찾아서 반환해줌.
		static std::weak_ptr<GameObject> Find(const std::string& name)
		{
			return Scene::GetInstance().Find(name);
		}

	private:
		Transform 		mTransform;
		Component_Map 	mComponentMap;
		std::string		mTag;
		bool 			mActiveSelf;
		bool			mbStatic;
	};
}

