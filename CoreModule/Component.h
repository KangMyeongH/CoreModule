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
		explicit Component(GameObject* _owner) : m_Owner(_owner) {}
		~Component() override = default;
		Component(const Component&) = default;
		Component& operator=(const Component&) = default;
		Component(Component&&) = delete;
		Component& operator=(Component&&) = delete;

		// Component�� �����ϰ� �ִ� GameObject�� ��ȯ
		GameObject* Get_GameObject() const { return m_Owner; }
		void Set_Owner(GameObject* _owner) { m_Owner = _owner; }

		Transform& 	Get_Transform() const;

		virtual Component* Clone(GameObject* _newOwner) const = 0;


	protected:
		GameObject* m_Owner;
	};

}
