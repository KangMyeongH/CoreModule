#pragma once
#include <string>
#include <memory>
#include <atomic>
#include "core_define.h"

namespace GameEngine
{
    class COREMODULE_API Object : public std::enable_shared_from_this<Object>
    {
    public:
        //======================================//
        //				constructor				//
        //======================================//
        explicit Object(std::string name = "Unnamed Object") : mID(sIDGenerator++), mName(std::move(name)), mIsDestroyed(false) {}
        virtual ~Object() = default;
        Object(const Object& rhs) : mID(sIDGenerator++), mName(rhs.mName), mIsDestroyed(rhs.mIsDestroyed) {}
        Object(Object&& rhs) noexcept
        {
            mID = rhs.mID;
            mName = rhs.mName;
            mIsDestroyed = rhs.mIsDestroyed;
            rhs.mID = NULL;
            rhs.mName = {};
            rhs.mIsDestroyed = NULL;
        }
        Object& operator=(const Object& rhs)
        {
            mID = sIDGenerator++;
            mName = rhs.mName;
            mIsDestroyed = rhs.mIsDestroyed;
            return *this;
        }
        Object& operator=(Object&& rhs) noexcept
        {
            mID = rhs.mID;
            mName = rhs.mName;
            mIsDestroyed = rhs.mIsDestroyed;
            rhs.mID = NULL;
            rhs.mName = {};
            rhs.mIsDestroyed = NULL;
            return *this;
        }

    public:
        //======================================//
        //				  method				//
        //======================================//
        void 			SetName(const std::string& name) { mName = name; }
        std::string 	GetName() const { return mName; }
        int 			GetInstanceID() const { return mID; }

    public:
        //======================================//
        //			   static method			//
        //======================================//
        static void 	Destroy(const std::shared_ptr<Object>& obj)
        {
            if (obj && !obj->mIsDestroyed)
            {
                // TODO : ���� ��� ť�� �ִ´�??
                // TODO : PendingDestroy �÷��׸� Ȱ��ȭ ���ش�?? �׷� ObjectŸ�Ժ��� ó������� �ϴµ�.... GameObject Component MonoBehaviour��
                obj->mIsDestroyed = true;
            }
        }

    private:
        static std::atomic<int> sIDGenerator; 	// ���� ID ������
        int 					mID;            // �� ��ü�� ���� ID
        std::string 			mName;          // ��ü �̸�
        bool 					mIsDestroyed;   // ���� �÷���
    };
}

