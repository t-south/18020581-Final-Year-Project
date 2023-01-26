#pragma once
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <system_error>
#include <iostream>


namespace GameEngine {
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
		static Window* instance;
		static void glfwError(int id, const char* description);
	};
}
