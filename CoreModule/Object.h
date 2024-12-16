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
                // TODO : ���� ��� ť�� �ִ´�??
                // TODO : PendingDestroy �÷��׸� Ȱ��ȭ ���ش�?? �׷� ObjectŸ�Ժ��� ó������� �ϴµ�.... GameObject Component MonoBehaviour��
                _obj->m_bDestroyed = true;
            }
        }

    private:
        static std::atomic<int> s_IdGenerator; 	// ���� ID ������
        int 					m_ID;            // �� ��ü�� ���� ID
        std::string 			m_Name;          // ��ü �̸�
        bool 					m_bDestroyed;   // ���� �÷���
    };
}

