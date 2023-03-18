#include "Window.h"


geProject::Window::Window(const char* m_title, int m_width, int m_height): title(m_title), width(m_width), height(m_height){		
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
	
	glfwSetWindowSizeCallback(window, window_size_callback);
	//current context
	glfwMakeContextCurrent(window);
	//v-sync enabled
	glfwSwapInterval(1);
	glfwShowWindow(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	//sceneManager = new SceneStates();

}

geProject::Window::~Window() {
	glfwDestroyWindow(window);
	glfwTerminate();	
}

void geProject::Window::glfwError(int id, const char* description){	std::cout << description << std::endl;}
void geProject::Window::window_size_callback(GLFWwindow* window, int width, int height){glfwSetWindowSize(window, width, height);}
GLFWwindow* geProject::Window::getWindow() {return window;}
int geProject::Window::getWidth() { return width; }
int geProject::Window::getHeight() { return height; }

//void geProject::Window::onUpdate(Event* event) {
	/*
	if (event->getType() == Type::gameStart) {
		std::cout << "Starting Play" << std::endl;
	}
	else if (event->getType() == Type::gameStop) {
		std::cout << "Stopping Play" << std::endl;
	}*/
//}

