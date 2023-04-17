#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ge_engine/Core.h>
#include <stdexcept>
#include <iostream>

namespace geProject {
	class Window
	{
	public:
		Window(const char* m_title, int m_width, int m_height);
		~Window();		
		GLFWwindow* getWindow();
		int getWidth();
		int getHeight();
		void stopWindow();
		static void window_size_callback(GLFWwindow* window, int width, int height);
	private:
		int width, height;
		const char* title;
		GLFWwindow* window;	
		static void glfwError(int id, const char* description);
	};
}
