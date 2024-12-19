#pragma once
#include "core_math.h"
#include "ComponentRegistrar.h"
#include "Object.h"

namespace GameEngine
{
	class GameObject;
	class Transform;

	class COREMODULE_API Component : public Object
	{
	public:
		explicit Component(GameObject* _owner) : m_Owner(_owner) {}
		~Component() override = default;
		Component(const Component&) = default;
		Component& operator=(const Component&) = default;
		Component(Component&&) = delete;
		Component& operator=(Component&&) = delete;

		// Component를 소유하고 있는 GameObject를 반환
		GameObject* Get_GameObject() const { return m_Owner; }
		void Set_Owner(GameObject* _owner) { m_Owner = _owner; }

		Transform& 	Get_Transform() const;

		virtual Component* Clone() const = 0;

	public:
		virtual void to_json(nlohmann::ordered_json& _j) = 0;
		virtual void from_json(const nlohmann::ordered_json& _j) = 0;

	protected:
		GameObject* m_Owner;
	};
}