#pragma once
#include <typeindex>
#include <unordered_map>

#include "BoxCollider.h"
#include "Collision.h"
#include "CollisionManager.h"
#include "Component.h"
#include "Object.h"
#include "Scene.h"
#include "Transform.h"
#include "CubeRenderer.h"
#include "MonoBehaviour.h"
#include "MonoBehaviourManager.h"
#include "PhysicsManager.h"
#include "RenderManager.h"
#include "Rigidbody.h"


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

			if (dynamic_cast<MonoBehaviour*>(component))
			{
				MonoBehaviourManager::GetInstance().Add_MonoBehaviour(dynamic_cast<MonoBehaviour*>(component));
				return component;
			}

			if (dynamic_cast<Rigidbody*>(component))
			{
				PhysicsManager::GetInstance().Add_Rigidbody(dynamic_cast<Rigidbody*>(component));
				return component;
			}

			if (dynamic_cast<BoxCollider*>(component))
			{
				CollisionManager::GetInstance().Add_Collider(dynamic_cast<Collider*>(component));
				return component;
			}

			if (dynamic_cast<Renderer*>(component))
			{
				RenderManager::GetInstance().Add_Renderer(dynamic_cast<Renderer*>(component));
				return component;
			}

			if (dynamic_cast<Light*>(component))
			{
				RenderManager::GetInstance().Add_Light(dynamic_cast<Light*>(component));
				return component;
			}

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

		void Set_Active(bool _active)
		{
			if (m_bActiveSelf != _active)
			{
				m_bActiveSelf = _active;
				for (Transform* child : m_Transform.Get_Children())
				{
					child->Get_GameObject()->Set_Active(_active);
				}
			}
		}

		void Set_Tag(const std::string& _tag) { m_Tag = _tag; }
		std::string Get_Tag() const { return m_Tag; }

		bool Is_Active() const { return m_bActiveSelf; }

		void Destroy() override;

		Component_Map& Get_ComponentMap() { return m_ComponentMap; }

		void On_CollisionEnter(Collision _other);
		void On_CollisionStay(Collision _other);
		void On_CollisionExit(Collision _other);

		//======================================//
		//			   static method			//
		//======================================//
		// 이름을 사용해서 현재 씬의 GameObject를 찾아서 반환해줌.
		static GameObject* Find(const std::string& _name)
		{
			return Scene::GetInstance().Find(_name);
		}


	public:
		friend void to_json(nlohmann::ordered_json& j, const GameObject& obj)
		{
			j = nlohmann::ordered_json
			{
				{"instanceID", obj.Get_InstanceID()},
				{"name", obj.Get_Name()},
				{"tag", obj.Get_Tag()},
				{"active", obj.Is_Active()},
				{"transform", obj.m_Transform},
				{"components", nlohmann::json::array()}
			};

			obj.ComponentToJson(j);
		}

		void ComponentToJson(nlohmann::ordered_json& _j) const
		{
			for (auto& component : m_ComponentMap)
			{
				nlohmann::ordered_json componentJson;
				component.second.front()->to_json(componentJson);
				_j["components"].push_back(componentJson);
			}
		}

		friend void from_json(const nlohmann::ordered_json& _j, GameObject& _obj)
		{
			_obj.Set_InstanceID(_j.at("instanceID").get<int>());
			std::string name;
			_j.at("name").get_to(name);
			_obj.Set_Name(name);
			_obj.Set_Tag(_j.at("tag").get<std::string>());
			_obj.Set_Active(_j.at("active").get<bool>());
			_j.at("transform").get_to(_obj.m_Transform);
			_obj.ComponentFromJson(_j);
		}

		void ComponentFromJson(const nlohmann::ordered_json& _j)
		{
			for (const auto& component_json : _j.at("components"))
			{
				// 어떤 컴포넌트인지 타입을 가져온 후
				std::string type = component_json.at("type").get<std::string>();

				// 타입에 맞는 컴포넌트를 만들고, 해당 정보를 넣는다.
				auto component = createComponent(type);
				component->from_json(component_json);
				component->Set_Owner(this);
				m_ComponentMap[typeid(*component)].push_back(component);
			}
		}

	private:
		Component* createComponent(const std::string& typeName)
		{
			auto& factory = ComponentFactory::Get_Instance().componentFactory;

			if (factory.find(typeName) != factory.end())
			{
				auto ptr = factory[typeName]();
				const auto& ref = *ptr;

				return ref.Clone();
			}
			return nullptr;
		}

	private:
		Transform 		m_Transform;
		Component_Map 	m_ComponentMap;
		std::string		m_Tag;
		bool 			m_bActiveSelf;
		bool			m_bStatic;
	};
}

