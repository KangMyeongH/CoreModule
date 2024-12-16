#pragma once
#include <string>
#include <memory>
#include <atomic>
#include "core_define.h"

namespace GameEngine
{
    class COREMODULE_API Object
    {
    public:
        //======================================//
        //				constructor				//
        //======================================//
        explicit Object(std::string _name = "Unnamed Object") : m_ID(s_IdGenerator++), m_Name(std::move(_name)), m_bDestroyed(false) {}
        virtual ~Object() = default;
        Object(const Object& _rhs) : m_ID(s_IdGenerator++), m_Name(_rhs.m_Name), m_bDestroyed(_rhs.m_bDestroyed) {}
        Object(Object&& _rhs) noexcept
        {
            m_ID = _rhs.m_ID;
            m_Name = _rhs.m_Name;
            m_bDestroyed = _rhs.m_bDestroyed;
            _rhs.m_ID = NULL;
            _rhs.m_Name = {};
            _rhs.m_bDestroyed = NULL;
        }
        Object& operator=(const Object& _rhs)
        {
            m_ID = s_IdGenerator++;
            m_Name = _rhs.m_Name;
            m_bDestroyed = _rhs.m_bDestroyed;
            return *this;
        }
        Object& operator=(Object&& _rhs) noexcept
        {
            m_ID = _rhs.m_ID;
            m_Name = _rhs.m_Name;
            m_bDestroyed = _rhs.m_bDestroyed;
            _rhs.m_ID = NULL;
            _rhs.m_Name = {};
            _rhs.m_bDestroyed = NULL;
            return *this;
        }

    public:
        //======================================//
        //				  method				//
        //======================================//
        void 			SetName(const std::string& _name) { m_Name = _name; }
        std::string 	GetName() const { return m_Name; }
        int 			GetInstanceID() const { return m_ID; }

    public:
        //======================================//
        //			   static method			//
        //======================================//
        static void 	Destroy(Object* _obj)
        {
            if (_obj && !_obj->m_bDestroyed)
            {
                // TODO : 삭제 대기 큐에 넣는다??
                // TODO : PendingDestroy 플래그만 활성화 해준다?? 그럼 Object타입별로 처리해줘야 하는데.... GameObject Component MonoBehaviour등
                _obj->m_bDestroyed = true;
            }
        }

    private:
        static std::atomic<int> s_IdGenerator; 	// 고유 ID 생성기
        int 					m_ID;            // 각 객체의 고유 ID
        std::string 			m_Name;          // 객체 이름
        bool 					m_bDestroyed;   // 삭제 플래그
    };
}

