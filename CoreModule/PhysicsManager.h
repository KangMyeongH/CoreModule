#pragma once

#include "core_define.h"



namespace GameEngine
{
	class Rigidbody;

	class COREMODULE_API PhysicsManager
	{
	private:
		//======================================//
		//				constructor				//
		//======================================//
		PhysicsManager() = default;
		~PhysicsManager();
	public:
		DECLARE_SINGLETON(PhysicsManager)

	public:
		//======================================//
		//				  method				//
		//======================================//
		void Physics_Update(float _deltaTime) const;
		void Add_Rigidbody(Rigidbody* _rigidbody);
		void Remove_Rigidbody(Rigidbody* _rigidbody);
		void Register_Rigidbody();
		void Destroy_Rigidbody();
		void Release();

	private:
		std::vector<Rigidbody*>		m_Rigidbodies;
		std::list<Rigidbody*>		m_RegisterQueue;
		std::list<Rigidbody*>		m_DestroyQueue;
	};
}

