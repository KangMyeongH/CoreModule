#pragma once
#include <typeindex>
#include <unordered_map>

#include "Object.h"

class Component;

using Component_Map = std::unordered_map<std::type_index, std::vector<std::shared_ptr<Component>>>;

namespace GameEngine
{
	class COREMODULE_API GameObject final : public Object
	{
	private:
		GameObject();

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

