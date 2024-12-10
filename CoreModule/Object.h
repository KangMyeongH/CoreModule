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
                // TODO : 삭제 대기 큐에 넣는다??
                // TODO : PendingDestroy 플래그만 활성화 해준다?? 그럼 Object타입별로 처리해줘야 하는데.... GameObject Component MonoBehaviour등
                obj->mIsDestroyed = true;
            }
        }

    private:
        static std::atomic<int> sIDGenerator; 	// 고유 ID 생성기
        int 					mID;            // 각 객체의 고유 ID
        std::string 			mName;          // 객체 이름
        bool 					mIsDestroyed;   // 삭제 플래그
    };
}

