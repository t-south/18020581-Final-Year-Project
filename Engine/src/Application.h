#pragma once
#include <ge_engine/Core.h>
#include "../../Engine/src/Render/Window.h"
#include <stdio.h>
#include <iostream>

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