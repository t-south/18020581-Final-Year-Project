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
		//add window to scene, also add mouse listener and keyboard listener
		sceneManager->getCurrentScene()->setWindow(gameWindow);
		sceneManager->getCurrentScene()->setCameraControlLayout();
		//mouse = MouseListener::getInstance();
		frameBuffer = new FrameBuffer(1920, 1080);
		selectionTextures = new FrameBuffer(1920, 1080, true);
		int width = gameWindow->getWidth();
		int height = gameWindow->getHeight();
		auto window = gameWindow->getWindow();
		imguiWindow = new ImguiWindow(window, width, height, frameBuffer);
		imguiWindow->start(width, height, sceneManager->getCurrentScene()->getMouseX(), sceneManager->getCurrentScene()->getMouseY());
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
			glDisable(GL_BLEND);
			selectionTextures->bindPicking();
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			auto scene = sceneManager->getCurrentScene();
			scene->render("../../../../Game/assets/shaders/SelectionVertexShader.glsl");
			auto mouse = scene->getMouseListener();
			if (mouse->mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
				int x = (int)mouse->getScreenXpos();
				int y = (int)mouse->getScreenYpos();
				int entityId = selectionTextures->getPixel(x, y);
				std::cout << "entity: " << entityId << std::endl;
				int activatedId = scene->getActiveEntity();
				if (entityId > -1) {
					if (activatedId != entityId) {
						scene->setActiveEntity(entityId);
						
					}


				}

				//scene->updateImgui();
			}

			selectionTextures->unBindPicking();
			glEnable(GL_BLEND);

			frameBuffer->bind();
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			/*if (keyboard->isKeyPressed(GLFW_KEY_SPACE)) {
				std::cout << "Space Key is pressed \n" << std::endl;
				sceneManager->switchScene(levelSceneId);
			}*/


			//SCENE UPDATES

			//add the view size and position of imgui game window to scene
			auto viewPos = imguiWindow->getViewPos();
			auto viewSize = imguiWindow->getViewSize();
			scene->setViewPos(viewPos.x, viewPos.y);
			scene->setViewSize(viewSize.x, viewSize.y);
			scene->update(deltaTime);
			scene->render("../../../../Game/assets/shaders/VertexShaderDefault.glsl");

			frameBuffer->unBind();
			//inverse for view has to be taken after render

			//IMGUI UPDATES
			imguiWindow->update(deltaTime, scene);
			if (loopCount > 0) {
				//scene->getCurrentScene()->update(deltaTime);
			}

			//imguiwindow->render(width, height);
			glfwSwapBuffers(gameWindow->getWindow());
			mouse->endFrame();
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


