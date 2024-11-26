#pragma once

namespace GameEngine
{
#pragma warning(disable : 4251)

#ifdef COREMODULE_EXPORTS
#define COREMODULE_API __declspec(dllexport)
#else
#define COREMODULE_API __declspec(dllimport)
#endif

#pragma region Singleton
#define NO_COPY(ClassName)							\
ClassName(const ClassName&) = delete; 				\
ClassName(ClassName&&) = delete;					\
ClassName& operator=(const ClassName&) = delete; 	\
ClassName& operator=(ClassName&&) = delete; 		

#define DECLARE_SINGLETON(ClassName)				\
		NO_COPY(ClassName)							\
public:												\
	static ClassName& GetInstance();


#define IMPLEMENT_SINGLETON(ClassName)				\
ClassName& ClassName::GetInstance()					\
{													\
	static ClassName sClass;						\
	return sClass;									\
}
#pragma endregion Singleton

}