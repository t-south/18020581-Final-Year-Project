#pragma once
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <system_error>
#include <iostream>
#include <ge_engine/KeyboardListener.h>
#include <ge_engine/MouseListener.h>


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
		static void glfwError(int id, const char* description);
		
	};
}
