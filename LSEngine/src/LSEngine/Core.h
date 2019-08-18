#pragma once

#ifdef LSE_PLATFORM_WINDOWS
	#ifdef LSE_BUILD_DLL
		#define LSE_API __declspec(dllexport)
	#else
		#define LSE_API __declspec(dllimport)
	#endif
#else
	#error Only Windows Supported
#endif

#ifdef LSE_ENABLE_ASSERTS
	#define LSE_ASSERT(x, ...) { if(!(x)) { LSE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define LSE_CORE_ASSERT(x, ...) { if(!(x)) { LSE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define LSE_ASSERT(x, ...)
	#define LSE_CORE_ASSERT(x, ...)
#endif

#define BIT(i) (1 << i)