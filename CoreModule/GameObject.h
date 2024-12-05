#pragma once
#include <typeindex>
#include <unordered_map>

#include "Component.h"
#include "Object.h"


// ���� ������Ʈ�� ������ ȣ�� ��� �ٷ�
// ���� ������Ʈ�� ������ ���� ȣ�� �� ��Ȱ��ȭ �� �������� ������ ��������
// ���� ������Ʈ�� Transform Component�� �ݵ�� ������.

namespace GameEngine
{
	class Component;
}

namespace GameEngine
{
	using Component_Map = std::unordered_map<std::type_index, std::vector<std::shared_ptr<Component>>>;

	class COREMODULE_API GameObject final : public Object
	{
	public:
		GameObject() = default;

	public:
		template <typename T, typename... Args>
		std::weak_ptr<T> AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
			std::shared_ptr<T> component = std::make_shared<T>(this, std::forward<Args>(args)...);
			mComponentMap[typeid(T)].push_back(component);

			return component;
		}

		template <typename T>
		std::weak_ptr<T> GetComponent()
		{
			auto it = mComponentMap.find(typeid(T));
			if (it != mComponentMap.end() && !it->second.empty())
			{
				return &it->second;
			}

			return nullptr;
		}

	private:
		Component_Map 	mComponentMap;
		bool 			mActiveSelf;		
	};
}

