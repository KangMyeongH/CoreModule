#pragma once
#include "Component.h"
#include "core_include.h"
#include "core_types.h"


//	1. 부모의 월드 트랜스폼이 변경된 경우 << worldDirtyFlags On 그리고 자식의 worldDirtyFlags On
//	2. 나의 로컬 트랜스폼이 변경된 경우   << 나의 worldDirtyFlags On
//	3. 나의 월드 트랜스폼이 변경된 경우   << 나의 localDirtyFlags on
//	4. 기존에 설정된 부모를 버리고 새로운 부모로 편입하는 경우 <<
//		4.1. 내가 최상단의 루트가 되는 경우 << 나의 local = world 자식의 dirty는 필요없다.
//		4.2. 새로운 부모가 편입 되는경우 << 나의 localDirtyFlags ON


namespace GameEngine
{
	class GameObject;

	enum TransformDirtyFlags_
	{	None = 0,				// 0000
		LocalDirty = 1 << 0,	// 0001 
		WorldDirty = 1 << 1,	// 0010
		ParentChanged = 1 << 2	// 0100
	};

	class COREMODULE_API Transform final : public Component
	{
	public:
		//======================================//
		//				constructor				//
		//======================================//

		Transform() : Component(nullptr), m_Parent(nullptr), m_DirtyFlags(None), m_bDirty(false) {}

		explicit Transform(GameObject* _owner) :
			Component(_owner), m_Parent(nullptr), m_LocalPosition(0.0f, 0.0f, 0.0f),
			m_LocalRotation(0.0f, 0.0f, 0.0f),
			m_LocalScale(1.f, 1.f, 1.f), m_DirtyFlags(WorldDirty), m_bDirty(true) {}

		~Transform() override = default;
		Transform(const Transform&) = default;
		Transform& operator=(const Transform&) = default;
		Transform(Transform&&) = delete;
		Transform& operator=(Transform&&) = delete;

		//======================================//
		//				 property				//
		//======================================//

		const Vector3& 		Get_LocalPosition() const { return m_LocalPosition; }
		void 				Set_LocalPosition(const Vector3& _position)
		{
			if (m_LocalPosition != _position)
			{
				m_LocalPosition = _position;
				set_Dirty();
			}
		}

		const Vector3& 		Get_LocalRotation() const { return m_LocalRotation; }
		void 				Set_LocalRotation(const Vector3& _rotation)
		{
			if (m_LocalRotation != _rotation)
			{
				m_LocalRotation = _rotation;

				float yaw = D3DXToRadian(m_LocalRotation.y);		// y축 회전 (Yaw)
				float pitch = D3DXToRadian(m_LocalRotation.x);	// x축 회전 (Pitch)
				float roll = D3DXToRadian(m_LocalRotation.z);		// z축 회전 (Roll)
				D3DXQuaternionRotationYawPitchRoll(&m_LocalQuaternionRotation, yaw, pitch, roll);

				set_Dirty();
			}
		}

		const Vector3& 		Get_LocalScale() const { return m_LocalScale; }
		void 				Set_LocalScale(const Vector3& _scale)
		{
			if (m_LocalScale != _scale)
			{
				m_LocalScale = _scale;

				set_Dirty();
			}
		}

		const Vector3& 		Position() const
		{
			update_MatrixIfNeeded();
			return m_WorldPosition;
		}
		void 				Set_Position(const Vector3& _position)
		{
			if (m_Parent)
			{
				D3DXMATRIX parentMat;
				D3DXMatrixInverse(&parentMat, nullptr, &m_Parent->Get_WorldMatrix());

				D3DXVECTOR4 localPosition;
				D3DXVec3Transform(&localPosition, &_position, &parentMat);
				Set_LocalPosition(Vector3(localPosition.x, localPosition.y, localPosition.z));
			}

			else
			{
				Set_LocalPosition(_position);
			}

			set_Dirty();
		}

		const Vector3& 		Rotation() const
		{
			update_MatrixIfNeeded();
			return m_WorldRotation;
		}
		void 				Set_Rotation(const Vector3& _rotation)
		{
			if (m_Parent)
			{
				Quaternion parentWorldRotation = m_Parent->Get_WorldQuaternionRotation();

				Quaternion parentInverseRotation;
				D3DXQuaternionInverse(&parentInverseRotation, &parentWorldRotation);

				float yaw = D3DXToRadian(_rotation.y);
				float pitch = D3DXToRadian(_rotation.x);
				float roll = D3DXToRadian(_rotation.z);

				Quaternion quaRot;
				D3DXQuaternionRotationYawPitchRoll(&quaRot, yaw, pitch, roll);

				D3DXQuaternionMultiply(&m_LocalQuaternionRotation, &parentInverseRotation, &quaRot);
				quaternionToEuler(m_LocalQuaternionRotation, m_LocalRotation);
			}

			else
			{
				Set_LocalRotation(_rotation);
			}

			set_Dirty();
		}

		const Vector3& 		Scale() const
		{
			update_MatrixIfNeeded();
			return m_WorldScale;
		}
		void				Set_Scale(const Vector3& _scale)
		{
			if (m_Parent)
			{
				Vector3 localScale = _scale / m_Parent->Scale();
				Set_LocalScale(localScale);
			}

			else
			{
				Set_LocalScale(_scale);
			}

			set_Dirty();
		}

		void 				Set_Parent(Transform* _parent)
		{
			if (m_Parent != _parent)
			{
				if (m_Parent != nullptr)
				{
					m_Parent->remove_Child(this);
				}

				m_Parent = _parent;

				m_LocalPosition = m_WorldPosition;
				m_LocalRotation = m_WorldRotation;
				m_LocalScale = m_WorldScale;

				if (m_Parent != nullptr)
				{
					D3DXMATRIX parentWorldMatrix = m_Parent->Get_WorldMatrix();
					D3DXMATRIX parentInverseMatrix;
					D3DXMatrixInverse(&parentInverseMatrix, nullptr, &parentWorldMatrix);

					// 월드 위치 -> 로컬 위치 변환
					D3DXVECTOR4 localPosition4;
					D3DXVec3Transform(&localPosition4, &m_WorldPosition, &parentInverseMatrix);
					m_LocalPosition = Vector3(localPosition4.x, localPosition4.y, localPosition4.z);

					// 월드 회전 -> 로컬 회전 변환
					D3DXQUATERNION parentWorldRotation = m_Parent->Get_WorldQuaternionRotation();
					D3DXQUATERNION parentInverseRotation;
					D3DXQuaternionInverse(&parentInverseRotation, &parentWorldRotation);
					D3DXQuaternionMultiply(&m_LocalQuaternionRotation, &parentInverseRotation, &m_WorldQuaternionRotation);

					quaternionToEuler(m_LocalQuaternionRotation, m_LocalRotation);

					m_LocalScale = m_WorldScale / m_Parent->Scale();

					//m_LocalPosition -= m_Parent->Get_LocalPosition();
					//m_LocalRotation -= m_Parent->Get_LocalRotation();

					m_Parent->add_Child(this);
				}

				set_Dirty();
			}
		}
		Transform* 			Get_Parent() const { return m_Parent; }

		const Quaternion&	Get_WorldQuaternionRotation() const
		{
			update_MatrixIfNeeded();
			return m_WorldQuaternionRotation;
		}

		const Quaternion&	Get_LocalQuaternionRotation() const
		{
			return m_LocalQuaternionRotation;
		}

		const D3DXMATRIX& 	Get_WorldMatrix()
		{
			update_MatrixIfNeeded();
			return m_WorldMatrix;
		}
		const std::vector<Transform*>& Get_Children() const { return m_Children; }

		//======================================//
		//				  method				//
		//======================================//

		void	Translate(const Vector3& _value);

	private:
		//======================================//
		//			  private method			//
		//======================================//

		void set_LocalDirty()
		{
			m_DirtyFlags |= LocalDirty;
		}

		void set_WorldDirty()
		{
			m_DirtyFlags |= WorldDirty;
		}

		void set_Dirty()
		{
			m_bDirty = true;

			if (!m_Children.empty())
			{
				for (auto& it : m_Children)
				{
					it->set_Dirty();
				}
			}
		}

		// world space를 새로 얻어야할 때
		void update_LocalToWorld()
		{
			if (!m_bDirty)
				return;

			D3DXMATRIX matScale, matRot, matTrans;

			// scale
			D3DXMatrixScaling(&matScale, m_LocalScale.x, m_LocalScale.y, m_LocalScale.z);

			// rotation
			D3DXQuaternionNormalize(&m_LocalQuaternionRotation, &m_LocalQuaternionRotation);
			D3DXMatrixRotationQuaternion(&matRot, &m_LocalQuaternionRotation);
			D3DXMatrixTranslation(&matTrans, m_LocalPosition.x, m_LocalPosition.y, m_LocalPosition.z);

			D3DXMATRIX matLocal = matScale * matRot * matTrans;

			if (m_Parent)
			{
				D3DXMATRIX matParent = m_Parent->Get_WorldMatrix();
				m_WorldMatrix = matParent * matLocal;

				D3DXMatrixDecompose(&m_WorldScale, &m_WorldQuaternionRotation, &m_WorldPosition, &m_WorldMatrix);

			}

			else
			{
				m_WorldMatrix = matLocal;
				m_WorldPosition = m_LocalPosition;
				m_WorldQuaternionRotation = m_LocalQuaternionRotation;
				m_WorldScale = m_LocalScale;
			}

			m_bDirty = false;
		}

		// local space를 새로 얻어야할 때
		void update_WorldToLocal()
		{
			if (!m_bDirty)
				return;
			
			if (m_Parent)
			{
				D3DXMATRIX matParent = m_Parent->Get_WorldMatrix();
				D3DXMatrixInverse(&matParent, nullptr, &matParent);

				D3DXMATRIX matLocal = matParent * m_WorldMatrix;
				D3DXMatrixDecompose(&m_LocalScale, &m_LocalQuaternionRotation, &m_LocalPosition, &matLocal);
			}

			else
			{
				m_LocalPosition = m_WorldPosition;
				m_LocalQuaternionRotation = m_WorldQuaternionRotation;
				m_LocalScale = m_WorldScale;
			}

			m_bDirty = false;
		}

		void add_Child(Transform* _child) { m_Children.push_back(_child); }
		void remove_Child(Transform* _child)
		{
			const auto it = std::remove(m_Children.begin(), m_Children.end(), _child);
			if (it != m_Children.end())
			{
				m_Children.erase(it, m_Children.end());
			}
		}

		void Destroy() override {}

		void quaternionToEuler(const Quaternion& quaternion, Vector3& euler)
		{
			// 쿼터니언 요소
			float x = quaternion.x;
			float y = quaternion.y;
			float z = quaternion.z;
			float w = quaternion.w;

			// Pitch (X축 회전)
			float sinPitch = 2.0f * (w * x - z * y);
			if (fabs(sinPitch) >= 1.0f) {
				// 특수 케이스: Gimble Lock 방지
				euler.x = D3DXToDegree(copysign(D3DX_PI / 2.0f, sinPitch)); // 90도 또는 -90도
			}
			else {
				euler.x = D3DXToDegree(asinf(sinPitch));
			}

			// Yaw (Y축 회전)
			euler.y = D3DXToDegree(atan2f(2.0f * (w * y + z * x), 1.0f - 2.0f * (x * x + y * y)));

			// Roll (Z축 회전)
			euler.z = D3DXToDegree(atan2f(2.0f * (w * z + x * y), 1.0f - 2.0f * (y * y + z * z)));
		}

		// 레거시 코드 안쓸 확률 높음
		void update_MatrixIfNeeded() const
		{
			if (!m_bDirty)
				return;

			// 로컬 행렬 계산
			D3DXMATRIX matScale, matRot, matTrans;

			// scale
			D3DXMatrixScaling(&matScale, m_LocalScale.x, m_LocalScale.y, m_LocalScale.z);

			// rotation
			float yaw = D3DXToRadian(m_LocalRotation.y);		// y축 회전 (Yaw)
			float pitch = D3DXToRadian(m_LocalRotation.x);	// x축 회전 (Pitch)
			float roll = D3DXToRadian(m_LocalRotation.z);		// z축 회전 (Roll)

			// 오일러 각 -> 쿼터니언 변환
			Quaternion quaternion;
			D3DXQuaternionRotationYawPitchRoll(&quaternion, yaw, pitch, roll);
			D3DXQuaternionNormalize(&quaternion, &quaternion);
			D3DXMatrixRotationQuaternion(&matRot, &quaternion);

			m_LocalQuaternionRotation = quaternion;

			// position
			D3DXMatrixTranslation(&matTrans, m_LocalPosition.x, m_LocalPosition.y, m_LocalPosition.z);

			D3DXMATRIX matLocal = matScale * matRot * matTrans;

			if (m_Parent)
			{
				D3DXMATRIX matParent = m_Parent->Get_WorldMatrix();
				m_WorldMatrix = matLocal * matParent;

				Quaternion quaternionRotation;
				D3DXMatrixDecompose(&m_WorldScale, &quaternionRotation, &m_WorldPosition, &m_WorldMatrix);

				m_WorldQuaternionRotation = quaternionRotation;

				D3DXMATRIX rotMat;
				D3DXMatrixRotationQuaternion(&rotMat, &quaternionRotation);

				m_WorldRotation.z = atan2(rotMat._32, rotMat._33); // roll
				m_WorldRotation.x = atan2(-rotMat._31, sqrt(rotMat._32 * rotMat._32 + rotMat._33 * rotMat._33)); // pitch
				m_WorldRotation.y = atan2(rotMat._21, rotMat._11); // Yaw

				m_WorldRotation.x = D3DXToDegree(m_WorldRotation.x);
				m_WorldRotation.y = D3DXToDegree(m_WorldRotation.y);
				m_WorldRotation.z = D3DXToDegree(m_WorldRotation.z);
			}

			else
			{
				m_WorldMatrix = matLocal;
				m_WorldPosition = m_LocalPosition;
				m_WorldRotation = m_LocalRotation;
				m_WorldScale = m_LocalScale;

				m_WorldQuaternionRotation = m_LocalQuaternionRotation;
			}

			m_bDirty = false;
		}


	public:
		void to_json(nlohmann::ordered_json& _j) override
		{
			_j = nlohmann::ordered_json{
				{"position", Get_LocalPosition()},
				{"rotation", Get_LocalRotation()},
				{"scale", Get_LocalScale()}
			};
		}

		void from_json(const nlohmann::ordered_json& _j) override
		{
			Set_LocalPosition(_j.at("position").get<Vector3>());
			Set_LocalRotation(_j.at("rotation").get<Vector3>());
			Set_LocalScale(_j.at("scale").get<Vector3>());
		}

		
		friend void to_json(nlohmann::ordered_json& j, const Transform& t)
		{
			int parentID = -1;
			if (t.m_Parent)
			{
				parentID = t.m_Parent->Get_InstanceID();
			}

			j = nlohmann::ordered_json{
				{"parentID", parentID},
				{"instanceID", t.Get_InstanceID()},
				{"position", t.Get_LocalPosition()},
				{"rotation", t.Get_LocalRotation()},
				{"scale", t.Scale()}
			};
		}

		friend void from_json(const nlohmann::ordered_json& j, Transform& t)
		{
			int parentID;
			j.at("parentID").get_to(parentID);
			if (parentID != -1)
			{
				t.s_ChildTransformMap[parentID].push_back(&t);
			}

			t.Set_InstanceID(j.at("instanceID").get<int>());
			s_TransformMap[t.Get_InstanceID()] = &t;
			t.Set_LocalPosition(j.at("position").get<Vector3>());
			t.Set_LocalRotation(j.at("rotation").get<Vector3>());
			//j.at("rotation").get_to(t.m_WorldRotation);
			t.Set_Scale(j.at("scale").get<Vector3>());
		}

		Component* Clone() const override
		{
			return new Transform(*this);
		}

	private:
		static std::unordered_map<int, Transform*> s_TransformMap;
		static std::unordered_map<int, std::vector<Transform*>> s_ChildTransformMap;

		Transform*					m_Parent;
		std::vector<Transform*> 	m_Children;

		// world space
		mutable D3DXMATRIX 	m_WorldMatrix;
		mutable Vector3 	m_WorldPosition;
		mutable Quaternion 	m_WorldQuaternionRotation;
		mutable Vector3 	m_WorldRotation;
		mutable Vector3 	m_WorldScale;

		// local space
		Vector3 			m_LocalPosition;
		mutable Quaternion	m_LocalQuaternionRotation;
		Vector3 			m_LocalRotation;
		Vector3 			m_LocalScale;

		TransformDirtyFlags m_DirtyFlags;

		mutable bool m_bDirty;

	};

	inline void Transform::Translate(const Vector3& _value)
	{
		if (_value.x != 0.f || _value.y != 0.f || _value.z != 0.f)
		{
			m_LocalPosition += _value;
			set_Dirty();
		}
	}

	REGISTER_COMPONENT(Transform)
}
