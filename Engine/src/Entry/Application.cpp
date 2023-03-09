#include "Application.h"





namespace geProject {
	Application::Application() {

	}
	Application::~Application() {


	}
	void Application::Startup() {		
		gameWindow = new geProject::Window("game", 1920, 1080);
		gameClock = Clock::getInstance();
		sceneManager = new SceneStates();
		sceneManager = new SceneStates();
		std::shared_ptr<Scene> levelEditorScene = std::make_shared<LevelEditorScene>();
		//levelEditorScene->setSerializer(&serial);
		std::shared_ptr<Scene> levelScene = std::make_shared<LevelScene>();
		int editorSceneId = sceneManager->addScene(levelEditorScene);
		int levelSceneId = sceneManager->addScene(levelScene);
		sceneManager->getCurrentScene()->setWindow(gameWindow);
		//mouse = MouseListener::getInstance();
		imguiWindow = new ImguiWindow(gameWindow->getWindow());
		imguiWindow->start(gameWindow->getWidth(), gameWindow->getHeight(), sceneManager->getCurrentScene()->getMouseX(), sceneManager->getCurrentScene()->getMouseY());
		loop();

		//game_window->loop();       
       
	}

	void Application::loop() {			
		gameClock->updateTime();
		//FPS variables
		float timePerSec = 0;
		int loopCount = 0;
		float deltaTime = 0;
		
		//SceneSerialize serial = SceneSerialize(sceneManager->getCurrentScene());
		//serial.deserialize("jsonTest.json");
		while (!glfwWindowShouldClose(gameWindow->getWindow())) {
			glfwPollEvents();
			
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			/*if (keyboard->isKeyPressed(GLFW_KEY_SPACE)) {
				std::cout << "Space Key is pressed \n" << std::endl;
				sceneManager->switchScene(levelSceneId);
			}*/
			//SCENE UPDATES
			auto scene = sceneManager->getCurrentScene();
			scene->update(deltaTime);
			//inverse for view has to be taken after render
			
			
			//IMGUI UPDATES
			imguiWindow->update(deltaTime, scene);
			if (loopCount > 0) {
				//scene->getCurrentScene()->update(deltaTime);
			}

			//imguiwindow->render(width, height);
			glfwSwapBuffers(gameWindow->getWindow());
			gameClock->updateTime();
			deltaTime = gameClock->getTime();
			timePerSec += deltaTime;
			loopCount++;
			//std::cout << "Frame time: " << deltaTime << std::endl;
			if (timePerSec > 1) {
				//std::cout << "FPS1: " << loopCount << std::endl;
				//std::cout << "FPS2: " << 1 / deltaTime << std::endl;
				timePerSec = 0;
				loopCount = 0;
			}
			gameClock->endFrame();
			gameClock->updateTime();
		}
		glfwDestroyWindow(gameWindow->getWindow());
		glfwTerminate();
		glfwSetErrorCallback(NULL);
	}
	


}