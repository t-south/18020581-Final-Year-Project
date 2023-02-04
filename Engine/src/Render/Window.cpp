#include "Window.h"



geProject::Window::Window(const char* m_title, int m_width, int m_height)
							: title(m_title), width(m_width), height(m_height)
{
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
	
	mouse = MouseListener::getInstance();
	keyboard = KeyboardListener::getInstance();
	
	glfwSetCursorPosCallback(window, mouse->cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse->mouse_button_callback);
	glfwSetScrollCallback(window, mouse->scroll_callback);
	glfwSetKeyCallback(window, keyboard->key_callback);
	

	//current context
	glfwMakeContextCurrent(window);
	//v-sync enabled
	glfwSwapInterval(1);
	glfwShowWindow(window);
}

geProject::Window::~Window() {
	glfwDestroyWindow(window);
	glfwTerminate();	
}



void geProject::Window::glfwError(int id, const char* description)
{
	std::cout << description << std::endl;
}

void geProject::Window::loop(){
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		if (keyboard->isKeyPressed(GLFW_KEY_SPACE)) {
			std::cout << "Space Key is pressed \n" << std::endl;
		}
		
		glfwSwapBuffers(window);
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	glfwSetErrorCallback(NULL);
}
