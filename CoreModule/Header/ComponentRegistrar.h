#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "core_define.h"
#include "ComponentFactory.h"

namespace GameEngine
{
	class Component;
}

class COREMODULE_API ComponentRegistrar
{
public:
	ComponentRegistrar(const std::string& _typeName, std::function<std::unique_ptr<GameEngine::Component>()> _creator) {
		ComponentFactory::Get_Instance().componentFactory[_typeName] = std::move(_creator);
	}
};