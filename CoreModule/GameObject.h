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

	using Component_Map = std::unordered_map<std::type_index, std::vector<Component*>>;

	class COREMODULE_API GameObject final : public Object
	{
	public:
		//======================================//
		//				constructor				//
		//======================================//

		GameObject();
		~GameObject() override;
		GameObject(const GameObject& _rhs) : Object(_rhs), m_Transform(_rhs.m_Transform)
		{
			m_Transform.Set_Owner(this);

			for (const auto& pair : _rhs.m_ComponentMap)
			{
				const auto& type = pair.first;
				const auto& components = pair.second;
				std::vector<Component*> clonedComponents;
				for (const auto& component : components)
				{
					component->Set_Owner(this);
					clonedComponents.push_back(component);
				}
				m_ComponentMap[type] = std::move(clonedComponents);
			}
		}

	public:
		//======================================//
		//				  method				//
		//======================================//

		// Component management
		template <typename T, typename... Args>
		T* Add_Component(Args&&... _args)
		{
			static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
			T* component = new T(this, std::forward<Args>(_args)...);
			m_ComponentMap[typeid(T)].push_back(component);
			// 각각의 Component매니저에 주소값 뿌리기.

			return component;
		}
		template <typename T>
		T* Get_Component()
		{
			auto it = m_ComponentMap.find(typeid(T));
			if (it != m_ComponentMap.end() && !it->second.empty())
			{
				return static_cast<T*>(it->second.front());
			}
			return nullptr;
		}
		template <typename T>
		std::vector<T*>* Get_Components() const
		{
			auto it = m_ComponentMap.find(typeid(T));
			if (it != m_ComponentMap.end() && !it->second.empty())
			{
				return &it->second;
			}
			return nullptr;
		}

		void Remove_Component(const Component* _component)
		{
			for(auto it = m_ComponentMap.begin(); it != m_ComponentMap.end();)
			{
				if (!it->second.empty() && it->second.front() == _component)
				{
					m_ComponentMap.erase(it);
					return;
				}

				++it;
			}

		}
		Transform& Get_Transform() { return m_Transform; }

		//======================================//
		//			   static method			//
		//======================================//
		// 이름을 사용해서 현재 씬의 GameObject를 찾아서 반환해줌.
		static GameObject* Find(const std::string& _name)
		{
			return Scene::GetInstance().Find(_name);
		}

	private:
		Transform 		m_Transform;
		Component_Map 	m_ComponentMap;
		std::string		m_Tag;
		bool 			m_bActiveSelf;
		bool			m_bStatic;
	};
}

