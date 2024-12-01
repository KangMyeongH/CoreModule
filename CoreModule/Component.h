#pragma once
#include "Object.h"

namespace GameEngine
{
	class GameObject;

	class COREMODULE_API Component : public Object
	{
	public:
		explicit Component(const std::weak_ptr<GameObject>& owner) : mOwner(owner) {}
		~Component() override = default;

	protected:
		std::weak_ptr<GameObject> mOwner;
	};

}
