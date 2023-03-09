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

void geProject::Window::glfwError(int id, const char* description){
	std::cout << description << std::endl;
}

/*
void geProject::Window::loop(){	
	auto clock = Clock::getInstance();
	sceneManager = new SceneStates();
	std::shared_ptr<Scene> levelEditorScene = std::make_shared<LevelEditorScene>();	
	//levelEditorScene->setSerializer(&serial);
	std::shared_ptr<Scene> levelScene = std::make_shared<LevelScene>();
	int editorSceneId = sceneManager->addScene(levelEditorScene);
    int levelSceneId = sceneManager->addScene(levelScene);
	clock->updateTime();
	//FPS variables
	float timePerSec = 0;
	int loopCount = 0;
	float deltaTime = 0;
	//SceneSerialize serial = SceneSerialize(sceneManager->getCurrentScene());
	//serial.deserialize("jsonTest.json");
	while (!glfwWindowShouldClose(window)){
		glfwPollEvents();
		glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);	
		if (keyboard->isKeyPressed(GLFW_KEY_SPACE)) {
			std::cout << "Space Key is pressed \n" << std::endl;
			sceneManager->switchScene(levelSceneId);			
		}
		//SCENE UPDATES
		auto scene = sceneManager->getCurrentScene();
		scene->update(deltaTime);
		//inverse for view has to be taken after render
		mouse->setInverses(scene->getCamera()->getProjectionInverse(), scene->getCamera()->getViewMatrixInverse());		
		scene->setWorldCoords(geProject::MouseListener::getInstance()->getCameraXpos(), geProject::MouseListener::getInstance()->getCameraYpos());
		//IMGUI UPDATES
		imguiwindow->update(deltaTime, scene);		
		if (loopCount > 0) {
			//scene->getCurrentScene()->update(deltaTime);
		}

		//imguiwindow->render(width, height);
		glfwSwapBuffers(window);
		clock->updateTime();
		deltaTime = clock->getTime();
		timePerSec += deltaTime;
		loopCount++;		
		//std::cout << "Frame time: " << deltaTime << std::endl;
		if (timePerSec > 1) {
			//std::cout << "FPS1: " << loopCount << std::endl;
			//std::cout << "FPS2: " << 1 / deltaTime << std::endl;
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
*/
/*
std::shared_ptr<geProject::Scene> geProject::Window::getScene() {
	return sceneManager->getCurrentScene();
}
*/

void geProject::Window::window_size_callback(GLFWwindow* window, int width, int height){
	glfwSetWindowSize(window, width, height);
}

GLFWwindow* geProject::Window::getWindow() {
	return window;
}

int geProject::Window::getWidth() { return width; }
int geProject::Window::getHeight() { return height; }