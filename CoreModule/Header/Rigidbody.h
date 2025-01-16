#pragma once
#include "Component.h"
namespace GameEngine
{
	class COREMODULE_API Rigidbody : public Component
	{
	public:
		//======================================//
		//				constructor				//
		//======================================//

		Rigidbody() : Component(nullptr), m_Velocity(0.f, 0.f, 0.f), m_Mass(1.f), m_Drag(0.f), m_AngularDrag(0.05f),
		              m_UseGravity(false), m_IsKinematic(false)
		{
		}

		explicit Rigidbody(GameObject* _owner) : Component(_owner), m_Velocity(0.f, 0.f, 0.f), m_Mass(1.f), m_Drag(0.f),
		                                         m_AngularDrag(0.05f),
		                                         m_UseGravity(false), m_IsKinematic(false)
		{
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_mass">무게(float)</param>
		/// <param name="_drag">공기 저항(float)</param>
		/// <param name="_angularDrag">공기 각 저항(float)</param>
		/// <param name="_useGravity">중력 사용 여부(bool)</param>
		/// <param name="_isKinematic">충돌 관련 물리 적용 여부(bool)</param>
		explicit Rigidbody(GameObject* _owner, const float _mass, const float _drag, const float _angularDrag, const bool _useGravity, const bool _isKinematic) :
			Component(_owner), m_Velocity(0.f, 0.f, 0.f),
			m_Mass(_mass), m_Drag(_drag), m_AngularDrag(_angularDrag), m_UseGravity(_useGravity), m_IsKinematic(_isKinematic)
		{
		}

		~Rigidbody() override = default;
		Rigidbody(const Rigidbody& _rhs) = default;
		Rigidbody& operator=(const Rigidbody& _rhs) = default;

		//======================================//
		//				 property				//
		//======================================//

		void		Set_Velocity(const Vector3& _velocity) { m_Velocity = _velocity; }
		Vector3		Get_Velocity() const { return m_Velocity; }
		Vector3& 	Velocity() { return m_Velocity; }

		void		Set_Mass(const float _mass) { m_Mass = _mass; }
		float		Get_Mass() const { return m_Mass; }

		void		Set_Drag(const float _drag) { m_Drag = _drag; }
		float		Get_Drag() const { return m_Drag; }

		void		Set_AngularDrag(const float _angularDrag) { m_AngularDrag = _angularDrag; }
		float		Get_AngularDrag() const { return m_AngularDrag; }

		void		Set_UseGravity(const bool _useGravity) { m_UseGravity = _useGravity; }
		bool		Get_UseGravity() const { return  m_UseGravity; }

		void		Set_IsKinematic(const bool _isKinematic) { m_IsKinematic = _isKinematic; }
		bool		Get_IsKinematic() const { return m_IsKinematic; }

		//======================================//
		//				 method					//
		//======================================//

		void		Add_Force(const Vector3& _force);
		void		Rigidbody_Update(const float& _deltaTime);

		Component* Clone() const override
		{
			return new Rigidbody(*this);
		}

		void Destroy() override;


		//======================================//
		//				 serialize				//
		//======================================//

		void to_json(nlohmann::ordered_json& _j) override
		{
			std::string type = "Rigidbody";

			_j = nlohmann::ordered_json{
				{"type", type},
				{"mass", Get_Mass()},
				{"drag", Get_Drag()},
				{"angularDrag", Get_AngularDrag()},
				{"useGravity", Get_UseGravity()},
				{"isKinematic", Get_IsKinematic()}
			};
		}

		void from_json(const nlohmann::ordered_json& _j) override
		{
			if (_j.contains("mass"))
				Set_Mass(_j.at("mass").get<float>());
			if (_j.contains("drag"))
				Set_Drag(_j.at("drag").get<float>());
			if (_j.contains("angularDrag"))
				Set_AngularDrag(_j.at("angularDrag").get<float>());
			if (_j.contains("useGravity"))
				Set_UseGravity(_j.at("useGravity").get<bool>());
			if (_j.contains("isKinematic"))
				Set_IsKinematic(_j.at("isKinematic").get<bool>());
		}

	private:
		Vector3		m_Velocity;
		float		m_Mass;
		float		m_Drag;
		float		m_AngularDrag;
		bool		m_UseGravity;
		bool		m_IsKinematic;
	};

	REGISTER_COMPONENT(Rigidbody)
}

