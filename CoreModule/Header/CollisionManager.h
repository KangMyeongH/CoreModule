#pragma once
#include <unordered_set>

#include "core_define.h"


namespace GameEngine
{
	class BoxCollider;

	struct ColliderPairHash
	{
		size_t operator()(const std::pair<BoxCollider*, BoxCollider*>& val) const
		{
			auto p1 = (val.first < val.second) ? val.first : val.second;
			auto p2 = (val.first < val.second) ? val.second : val.first;

			return std::hash<void*>()(p1) ^ std::hash<void*>()(p2);
		}
	};

	struct ColliderPairEq
	{
		bool operator()(const std::pair<BoxCollider*, BoxCollider*>& a,
			const std::pair<BoxCollider*, BoxCollider*>& b) const
		{
			// 정규화(A < B) 후 동일한지
			BoxCollider* a1 = (a.first < a.second) ? a.first : a.second;
			BoxCollider* a2 = (a.first < a.second) ? a.second : a.first;

			BoxCollider* b1 = (b.first < b.second) ? b.first : b.second;
			BoxCollider* b2 = (b.first < b.second) ? b.second : b.first;

			return (a1 == b1 && a2 == b2);
		}
	};

}

namespace GameEngine
{
	class Collider;

	class COREMODULE_API CollisionManager
	{
	private:
		//======================================//
		//				constructor				//
		//======================================//
		CollisionManager() = default;
		~CollisionManager();
	public:
		DECLARE_SINGLETON(CollisionManager)

	public:
		//======================================//
		//				  method				//
		//======================================//

		void Collider_Update();
		void Add_Collider(Collider* _collider);
		void Remove_Collider(Collider* _collider);
		void Register_Collider();
		void Destroy_Collider();
		void Release();

	private:
		// 1) BroadPhase: SAP
		void broadPhase_Sap(std::vector<std::pair<BoxCollider*, BoxCollider*>>& _outPotentialPairs);

		// 2) NarrowPhase: OBB 충돌
		void narrowPhase_OBB(const std::vector<std::pair<BoxCollider*, BoxCollider*>>& _potentialPairs);

		float 	get_OBBRadiusOnAxis(const BoxCollider* _box, const Vector3& _axis);
		bool 	is_AxisSeparating(const BoxCollider* _a, const BoxCollider* _b, const Vector3& _axis);
		bool 	check_OBBCollision(const BoxCollider* _a, const BoxCollider* _b);

		// 충돌 발생 시 처리
		void process_CollisionResults(const std::unordered_set<std::pair<BoxCollider*, BoxCollider*>, ColliderPairHash, ColliderPairEq>& newCollisions);;


		void invoke_CollisionEnter(BoxCollider* _a, BoxCollider* _b);
		void invoke_CollisionStay(BoxCollider* _a, BoxCollider* _b);
		void invoke_CollisionExit(BoxCollider* _a, BoxCollider* _b);

	private:
		std::vector<Collider*> 		m_Colliders;
		std::unordered_set<std::pair<BoxCollider*, BoxCollider*>, ColliderPairHash, ColliderPairEq> m_CollisionPairs;
		std::list<Collider*>		m_RegisterQueue;
		std::list<Collider*>		m_DestroyQueue;
	};

}
