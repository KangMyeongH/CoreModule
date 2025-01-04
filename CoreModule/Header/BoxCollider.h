#pragma once
#include "Collider.h"

/// <summary>
/// 1. AABB으로 먼저 OBB충돌 검사를 해줄 객체들을 고른다.
/// 
/// 2. OBB를 구한다.
///
///	3. OBB OBB 충돌 검사 및, OBB Sphere 충돌 검사
/// </summary>

namespace GameEngine
{
	class SphereCollider;

    class COREMODULE_API BoxCollider final : public Collider
    {
    public:
		BoxCollider() : Collider(ColliderType::Box), m_Size(1.0f, 1.0f, 1.0f) {}
		explicit BoxCollider(GameObject* _owner) : Collider(_owner, ColliderType::Box), m_Size(1.0f, 1.0f, 1.0f) {}
		~BoxCollider() override = default;
		BoxCollider(const BoxCollider& _rhs) : Collider(_rhs), m_Center(_rhs.m_Center), m_Size(_rhs.m_Size) { m_Type = ColliderType::Box; }

		void		Update_Collider() override;
		Vector3 	Get_Center() const { return m_Center; }
		void		Set_Center(const Vector3& _center) { m_Center = _center; }
		Vector3		Get_Size() const { return m_Size; }
		void		Set_Size(const Vector3& _size) { m_Size = _size; }

		Vector3		Get_WorldMin() const { return m_WorldMin; }
		Vector3		Get_WorldMax() const { return m_WorldMax; }

		OBB			Get_OBB() const { return m_OBB; }

		bool 		Check_Collision(Collider* _other, Vector3& _contactPoint) override;

		OBB			Calc_WorldOBB();
		void		Calc_WorldAABB();

    private:
		bool		Check_OBBCollision(BoxCollider* _other, Vector3& _contactPoint);
		bool		Check_SphereCollision(SphereCollider* _other, Vector3& _contactPoint);

    public:
	    void to_json(nlohmann::ordered_json& _j) override
	    {
			std::string type = "BoxCollider";
			_j = nlohmann::ordered_json{
				{"type", type},
				{"enable", m_bEnabled},
				{"colliderType", m_Type},
				{"isTrigger", m_bTrigger},
				{"center", m_Center},
				{"size", m_Size}
			};
	    }
	    void from_json(const nlohmann::ordered_json& _j) override
	    {
			_j.at("enable").get_to(m_bEnabled);
			_j.at("colliderType").get_to(m_Type);
			_j.at("isTrigger").get_to(m_bTrigger);
			_j.at("center").get_to(m_Center);
			_j.at("size").get_to(m_Size);
	    }

	    Component* Clone() const override
	    {
			return new BoxCollider(*this);
	    }

		void Debug_Draw(LPDIRECT3DDEVICE9 _device, DWORD _color) override;

	private:
		OBB			m_OBB;
		Vector3 	m_Center;
		Vector3 	m_Size;
		Vector3		m_WorldMin;
		Vector3		m_WorldMax;
    };

	REGISTER_COMPONENT(BoxCollider)
}
