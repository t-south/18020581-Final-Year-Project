#include "Window.h"



geProject::Window::Window(const char* m_title, int m_width, int m_height): title(m_title), width(m_width), height(m_height)
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
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
	auto clock = Clock::getInstance();
	auto scene = new SceneStates();
	std::shared_ptr<Scene> levelEditorScene = std::make_shared<LevelEditorScene>();
	std::shared_ptr<Scene> levelScene = std::make_shared<LevelScene>();
	int editorSceneId = scene->addScene(levelEditorScene);
    int levelSceneId = scene->addScene(levelScene);
	clock->updateTime();
	//FPS variables
	float timePerSec = 0;
	int loopCount = 0;
	float deltaTime = 0;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		if (keyboard->isKeyPressed(GLFW_KEY_SPACE)) {
			std::cout << "Space Key is pressed \n" << std::endl;
			//scene->switchScene(levelSceneId);
			
		}
		scene->getCurrentScene()->update(deltaTime);

		if (loopCount > 0) {
			//scene->getCurrentScene()->update(deltaTime);
		}

		glfwSwapBuffers(window);
		clock->updateTime();
		deltaTime = clock->getTime();
		timePerSec += deltaTime;
		loopCount++;

		//std::cout << "Frame time: " << deltaTime << std::endl;
		if (timePerSec > 1) {
			std::cout << "FPS1: " << loopCount << std::endl;
			std::cout << "FPS2: " << 1 / deltaTime << std::endl;
			timePerSec = 0;
			loopCount = 0;
		}
		
		clock->endFrame();
		clock->updateTime();
		
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	glfwSetErrorCallback(NULL);

}
