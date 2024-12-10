#pragma once
#include "Component.h"

namespace GameEngine
{
	class GameObject;

	class COREMODULE_API Transform final : public Component
	{
	public:
		explicit Transform(const std::weak_ptr<GameObject>& owner) : Component(owner) {}
		~Transform() override = default;
		Transform(const Transform& rhs) = default;
		Transform& operator=(const Transform& rhs) = default;
		Transform(Transform&&) = delete;
		Transform& operator=(Transform&&) = delete;

		std::shared_ptr<Component> Clone(GameObject* newOwner) const override;

	private:
		Vector3 mPosition;
		Vector3 mRotation;
		Vector3 mScale;
	};

	inline std::shared_ptr<Component> Transform::Clone(GameObject* newOwner) const
	{
		return std::make_shared<Transform>(*this);
	}
}
