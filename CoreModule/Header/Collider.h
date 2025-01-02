#pragma once
#include "Behaviour.h"

namespace GameEngine
{
    class COREMODULE_API Collider : public Behaviour
    {
    public:
		Collider(const ColliderType _type) : Behaviour(nullptr), m_Type(_type), m_bTrigger(false)
		{}

	    explicit Collider(GameObject* _owner, const ColliderType _type)
			: Behaviour(_owner), m_Type(_type), m_bTrigger(false)
		{}

        Collider(const Collider& _rhs) = default;

		~Collider() override = default;

    public:
        virtual void    Update_Collider() = 0;
    	virtual bool 	Check_Collision(Collider* _other, Vector3& _contactPoint) = 0;
        ColliderType    Get_ColliderType() const { return m_Type; }
        void            Set_ColliderType(const ColliderType& _type) { m_Type = _type; }
        bool 			Is_Trigger() const { return m_bTrigger; }
        void 			Set_Trigger(const bool _trigger) { m_bTrigger = _trigger; }

        void 			Destroy() override;
		void 			to_json(nlohmann::ordered_json& _j) override = 0;
		void 			from_json(const nlohmann::ordered_json& _j) override = 0;
		Component* 		Clone() const override = 0;

	protected:
        ColliderType 	m_Type;
        bool            m_bTrigger;
    };
}


