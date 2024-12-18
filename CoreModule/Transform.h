#pragma once
#include "Component.h"
#include "core_include.h"
#include "core_types.h"


//	1. �θ��� ���� Ʈ�������� ����� ��� << worldDirtyFlags On �׸��� �ڽ��� worldDirtyFlags On
//	2. ���� ���� Ʈ�������� ����� ���   << ���� worldDirtyFlags On
//	3. ���� ���� Ʈ�������� ����� ���   << ���� localDirtyFlags on
//	4. ������ ������ �θ� ������ ���ο� �θ�� �����ϴ� ��� <<
//		4.1. ���� �ֻ���� ��Ʈ�� �Ǵ� ��� << ���� local = world �ڽ��� dirty�� �ʿ����.
//		4.2. ���ο� �θ� ���� �Ǵ°�� << ���� localDirtyFlags ON


namespace GameEngine
{
	class GameObject;

	class COREMODULE_API Transform final : public Component
	{
	public:
		//======================================//
		//				constructor				//
		//======================================//
		explicit Transform(GameObject* _owner) :
			Component(_owner), m_Parent(nullptr), m_LocalPosition(0.0f, 0.0f, 0.0f),
			m_LocalRotation(0.0f, 0.0f, 0.0f),
			m_LocalScale(1.f, 1.f, 1.f), m_DirtyFlags(TransformDirtyFlags::WorldDirty), m_bDirty(true) {}

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

		const Vector3& 		Position() const { return m_WorldPosition; }
		void 				Set_Position(const Vector3& _position)
		{
			if (m_WorldPosition != _position)
			{
				if (m_Parent)
				{
					Set_LocalPosition(_position - m_Parent->Position());
				}

				else
				{
					Set_LocalPosition(_position);
				}
			}
		}
		const Vector3& 		Rotation() const { return m_WorldRotation; }
		void 				Set_Rotation(const Vector3& _rotation)
		{
			if (m_WorldRotation != _rotation)
			{
				if (m_Parent)
				{
					Set_LocalRotation(_rotation - m_Parent->Rotation());
				}

				else
				{
					Set_LocalRotation(_rotation);
				}
			}
		}
		const Vector3& 		Scale() const { return m_WorldScale; }

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

				if (m_Parent != nullptr)
				{
					m_LocalPosition -= m_Parent->Get_LocalPosition();
					m_LocalRotation -= m_Parent->Get_LocalRotation();

					m_Parent->add_Child(this);
				}

				set_Dirty();
			}
		}
		Transform* 			Get_Parent() const { return m_Parent; }
		const D3DXMATRIX& 	Get_WorldMatrix()
		{
			update_MatrixIfNeeded();
			return m_WorldMatrix;
		}
		const std::vector<Transform*>& Get_Children() const { return m_Children; }


		//======================================//
		//				  method				//
		//======================================//

	private:
		//======================================//
		//			  private method			//
		//======================================//
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

		// world space�� ���� ������ ��
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

		// local space�� ���� ������ ��
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

		// ���Ž� �ڵ� �Ⱦ� Ȯ�� ����
		void update_MatrixIfNeeded()
		{
			if (!m_bDirty)
				return;

			// ���� ��� ���
			D3DXMATRIX matScale, matRot, matTrans;

			// scale
			D3DXMatrixScaling(&matScale, m_LocalScale.x, m_LocalScale.y, m_LocalScale.z);

			// rotation
			float yaw = D3DXToRadian(m_LocalRotation.y);		// y�� ȸ�� (Yaw)
			float pitch = D3DXToRadian(m_LocalRotation.x);	// x�� ȸ�� (Pitch)
			float roll = D3DXToRadian(m_LocalRotation.z);		// z�� ȸ�� (Roll)

			// ���Ϸ� �� -> ���ʹϾ� ��ȯ
			D3DXQUATERNION quaternion;
			D3DXQuaternionRotationYawPitchRoll(&quaternion, yaw, pitch, roll);
			D3DXQuaternionNormalize(&quaternion, &quaternion);
			D3DXMatrixRotationQuaternion(&matRot, &quaternion);

			// position
			D3DXMatrixTranslation(&matTrans, m_LocalPosition.x, m_LocalPosition.y, m_LocalPosition.z);

			D3DXMATRIX matLocal = matScale * matRot * matTrans;

			if (m_Parent)
			{
				D3DXMATRIX matParent = m_Parent->Get_WorldMatrix();
				m_WorldMatrix = matLocal * matParent;

				D3DXQUATERNION quaternionRotation;
				D3DXMatrixDecompose(&m_WorldScale, &quaternionRotation, &m_WorldPosition, &m_WorldMatrix);

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
			}

			m_bDirty = false;
		}


	public:

		friend void to_json(nlohmann::json& j, const Transform& t)
		{
			j = nlohmann::json{
				{"position", t.m_WorldPosition},
				{"rotation", t.m_WorldRotation},
				{"scale", t.m_WorldScale}
			};
		}

		friend void from_json(const nlohmann::json& j, Transform& t)
		{
			j.at("position").get_to(t.m_WorldPosition);
			j.at("rotation").get_to(t.m_WorldRotation);
			j.at("scale").get_to(t.m_WorldScale);
		}

	private:
		Transform*					m_Parent;
		std::vector<Transform*> 	m_Children;

		// world space
		D3DXMATRIX 	m_WorldMatrix;
		Vector3 	m_WorldPosition;
		D3DXQUATERNION m_WorldQuaternionRotation;
		Vector3 	m_WorldRotation;
		Vector3 	m_WorldScale;

		// local space
		Vector3 	m_LocalPosition;
		D3DXQUATERNION m_LocalQuaternionRotation;
		Vector3 	m_LocalRotation;
		Vector3 	m_LocalScale;

		TransformDirtyFlags m_DirtyFlags;

		bool m_bDirty;
	};

	/*
	inline Component* Transform::Clone(GameObject* _newOwner) const
	{
		return new Transform(*this);
	}*/
}