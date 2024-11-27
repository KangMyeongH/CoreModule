#pragma once
#include <string>
#include <memory>
#include <atomic>

#include "CoreDefine.h"

namespace GameEngine
{
    class COREMODULE_API Object : public std::enable_shared_from_this<Object>
    {
    protected:
        explicit 	Object(std::string name = "Unnamed Object") : mID(sIDGenerator++), mName(std::move(name)), mIsDestroyed(false) {}
        virtual 	~Object() = default;

    public:
        NO_COPY(Object)

    public:
        void 			SetName(const std::string& name) { mName = name; }
        std::string 	GetName() const { return mName; }
        int 			GetInstanceID() const { return mID; }

        static void 	Destroy(std::shared_ptr<Object>& obj)
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

    std::atomic<int> Object::sIDGenerator{ 0 };
}

