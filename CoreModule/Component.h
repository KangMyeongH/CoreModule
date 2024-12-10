#pragma once
#include "core_math.h"
#include "Object.h"

namespace GameEngine
{
	class GameObject;
	class Transform;

	class COREMODULE_API Component : public Object
	{
	public:
		explicit Component(const std::weak_ptr<GameObject>& owner) : mOwner(owner) {}
		~Component() override = default;
		Component(const Component&) = default;
		Component& operator=(const Component& rhs) = default;
		Component(Component&&) = delete;
		Component& operator=(Component&&) = delete;

		// Component를 소유하고 있는 GameObject를 반환
		std::weak_ptr<GameObject> GetGameObject() const { return mOwner; }

		void SetOwner(const std::weak_ptr<GameObject>& owner) { mOwner = owner; }
		virtual std::shared_ptr<Component> Clone(GameObject* newOwner) const = 0;


	protected:
		std::weak_ptr<GameObject> mOwner;
	};

}
