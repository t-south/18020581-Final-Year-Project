#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <system_error>
#include <iostream>
#include <memory>
#include "../Inputs/MouseListener.h"
#include "../Inputs/KeyboardListener.h"
#include "../Clock/Clock.h"
#include "../SceneManager/SceneStates.h"
#include "../SceneManager/LevelEditorScene.h"
#include "../SceneManager/LevelScene.h"

namespace geProject {
	class Window
	{
	public:
		Window(const char* m_title, int m_width, int m_height);
		~Window();
		void loop();
		
	private:
		int width, height;
		const char* title;
		GLFWwindow* window;	
		MouseListener* mouse;
		KeyboardListener* keyboard;
		
		//Time* clock;
		static void glfwError(int id, const char* description);
		
	};
}
