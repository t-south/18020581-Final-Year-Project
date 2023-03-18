#pragma once
#include "../../src/Events/EventHandler.h"
#include "../../src/Events/Event.h"
extern geProject::EventHandler eventSystem;


#ifdef GE_PLATFORM_WINDOWS
	#ifdef GE_BUILD_DLL
		#define GE_API __declspec(dllexport)
	#else
		#define GE_API __declspec(dllimport)
	#endif // GE_BUILD_DLL

#endif // GE_PLATFORM_WINDOWS
