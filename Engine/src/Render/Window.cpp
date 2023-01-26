#include "Window.h"



GameEngine::Window::Window(const char* m_title, int m_width, int m_height): title(m_title), width(m_width), height(m_height){
	//enable glfw errors
	glfwSetErrorCallback(&glfwError);	
	if (!glfwInit())
		throw std::runtime_error("GLFW failed to initialize.");
	//provide hints for window
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	//create window
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if(window == NULL)
		throw std::runtime_error("GLFW failed to create window.");
	//current context
	glfwMakeContextCurrent(window);
	//v-sync enabled
	glfwSwapInterval(1);
	glfwShowWindow(window);
}

GameEngine::Window::~Window() {
	glfwDestroyWindow(window);
	glfwTerminate();	
}

void GameEngine::Window::glfwError(int id, const char* description)
{
	std::cout << description << std::endl;
}

void GameEngine::Window::loop(){
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.0f, 0.5f);
		glVertex2f(0.5f, -0.5f);
		glEnd();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
