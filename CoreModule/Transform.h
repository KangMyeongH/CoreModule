#pragma once
#include "Component.h"

namespace GameEngine
{
	class GameObject;

	class Transform final : public Component
	{
	public:
		explicit Transform(const std::weak_ptr<GameObject>& owner)
			: Component(owner)
		{
		}

	private:

	};

}
