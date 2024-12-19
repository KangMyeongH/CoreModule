#pragma once
#include <typeindex>
#include <unordered_map>

#include "Component.h"
#include "Object.h"
#include "Scene.h"
#include "Transform.h"


// ���� ������Ʈ�� ������ ȣ�� ��� �ٷ�
// ���� ������Ʈ�� ������ ���� ȣ�� �� ��Ȱ��ȭ �� �������� ������ ��������
// ���� ������Ʈ�� Transform Component�� �ݵ�� ������.


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
			// ������ Component�Ŵ����� �ּҰ� �Ѹ���.

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


		//======================================//
		//			   static method			//
		//======================================//
		// �̸��� ����ؼ� ���� ���� GameObject�� ã�Ƽ� ��ȯ����.
		static GameObject* Find(const std::string& _name)
		{
			return Scene::GetInstance().Find(_name);
		}


	public:
		friend void to_json(nlohmann::json& j, const GameObject& obj)
		{
			j = nlohmann::json
			{
				{"name", obj.Get_Name()},
				{"tag", obj.Get_Tag()},
				{"active", obj.Is_Active()},
				{"transform", obj.m_Transform},
				{"components", nlohmann::json::array()}
			};

			obj.ComponentToJson(j);
		}

		void ComponentToJson(nlohmann::json& _j) const
		{
			for (auto& component : m_ComponentMap)
			{
				nlohmann::json componentJson;
				component.second.front()->to_json(componentJson);
				_j["components"].push_back(componentJson);
			}
		}

		friend void from_json(const nlohmann::json& j, GameObject& obj)
		{
			std::string name;
			j.at("name").get_to(name);
			obj.Set_Name(name);
			j.at("transform").get_to(obj.m_Transform);
		}

		void ComponentFromJson(const nlohmann::json& j)
		{
			for (const auto& component_json : j.at("components"))
			{
				// � ������Ʈ���� Ÿ���� ������ ��
				std::string type = component_json.at("type").get<std::string>();

				// Ÿ�Կ� �´� ������Ʈ�� �����, �ش� ������ �ִ´�.
				auto component = createComponent(type);
				component->from_json(j);
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

