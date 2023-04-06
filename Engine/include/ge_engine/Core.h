#pragma once
#include "../../src/Events/EventHandler.h"
#include "../../src/Events/Event.h" 
#include "../../src/ResourceManager/ResourceManager.h"
#include "../../src/AI/WorldStates.h"




extern geProject::EventHandler eventSystem;
extern geProject::ResourceManager resourcemanager;
extern geProject::WorldState worldstate;


#ifdef GE_PLATFORM_WINDOWS
	#ifdef GE_BUILD_DLL
		#define GE_API __declspec(dllexport)
	#else
		#define GE_API __declspec(dllimport)
	#endif // GE_BUILD_DLL

#endif // GE_PLATFORM_WINDOWS
