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

#define BIT(i) (1 << i)

#include "Application.h"
#include "Log.h"