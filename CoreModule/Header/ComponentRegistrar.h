#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "core_define.h"
#include "ComponentFactory.h"

namespace GameEngine
{
	class Component;
}

class COREMODULE_API ComponentRegistrar
{
public:
	ComponentRegistrar(const std::string& typeName, std::function<std::unique_ptr<GameEngine::Component>()> creator) {
		ComponentFactory::Get_Instance().componentFactory[typeName] = creator;
	}
};