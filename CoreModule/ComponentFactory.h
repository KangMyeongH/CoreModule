#pragma once
#include "core_define.h"

class Component;

class COREMODULE_API ComponentFactory
{
public:
	static ComponentFactory& Get_Instance()
	{
		static ComponentFactory instance;
		return instance;
	}

	std::unordered_map<std::string, std::function<std::unique_ptr<GameEngine::Component>()>> componentFactory;

private:
	ComponentFactory() = default;
	~ComponentFactory() = default;
public:
	ComponentFactory(const ComponentFactory&) = delete;
	ComponentFactory& operator=(const ComponentFactory&) = delete;
	ComponentFactory(ComponentFactory&&) = delete;
	ComponentFactory& operator=(ComponentFactory&&) = delete;
};
