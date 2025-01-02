#pragma once
#include "Collider.h"

namespace GameEngine
{
	class COREMODULE_API SphereCollider : public Collider
	{
	public:
		SphereCollider()
			: Collider(ColliderType::Sphere), m_Radius(0.5f)
		{
		}

		explicit SphereCollider(GameObject* _owner)
			: Collider(_owner, ColliderType::Sphere), m_Radius(0.5f)
		{
		}

		SphereCollider(const SphereCollider& _rhs)
			: Collider(_rhs), m_Center(_rhs.m_Center), m_Radius(_rhs.m_Radius)
		{
			m_Type = ColliderType::Sphere;
		}

		~SphereCollider() override = default;

		void Update_Collider() override;
		bool Check_Collision(Collider* _other, Vector3& _contactPoint) override;

	public:
		void to_json(nlohmann::ordered_json& _j) override
		{
			std::string type = "SphereCollider";
			_j = nlohmann::ordered_json{
				{"type", type},
				{"enable", m_bEnabled},
				{"colliderType", m_Type},
				{"isTrigger", m_bTrigger},
				{"center", m_Center},
				{"radius", m_Radius}
			};
		}
		void from_json(const nlohmann::ordered_json& _j) override
		{
			_j.at("enable").get_to(m_bEnabled);
			_j.at("colliderType").get_to(m_Type);
			_j.at("isTrigger").get_to(m_bTrigger);
			_j.at("center").get_to(m_Center);
			_j.at("radius").get_to(m_Radius);
		}
		Component* Clone() const override
		{
			return new SphereCollider(*this);
		}

	private:
		Vector3 m_Center;
		float	m_Radius;
	};

	REGISTER_COMPONENT(SphereCollider)
}