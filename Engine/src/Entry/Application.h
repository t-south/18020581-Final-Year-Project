#pragma once
#include <ge_engine/Core.h>
#include <stdio.h>
#include <iostream>
#include "../../Engine/src/Render/Window.h"

namespace geProject {
	class Application
	{
	public:
		Application();
		virtual ~Application();
		void Startup();
	};

	Application* CreateApplication();

	
}