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

#include "Application.h"