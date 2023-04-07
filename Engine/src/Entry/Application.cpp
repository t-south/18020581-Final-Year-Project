#include "Application.h"

geProject::EventHandler eventSystem;
geProject::ResourceManager resourcemanager;
geProject::WorldState worldstate;
geProject::EntityManager entitymanager;
geProject::Physics physicsmanager;

namespace geProject {
	Application::Application() {

	}
	Application::~Application() {


	}

	void Application::Startup() {
		eventSystem = EventHandler();
		eventSystem.subscribe(this, &Application::startGamePlay);
		eventSystem.subscribe(this, &Application::stopGamePlay);
		eventSystem.subscribe(this, &Application::loadGame);
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
		//mouse = MouseListener::getInstance();
		frameBuffer = new FrameBuffer(1920, 1080);
		int width = gameWindow->getWidth();
		int height = gameWindow->getHeight();
		auto window = gameWindow->getWindow();
		imguiWindow = new ImguiWindow(window, width, height, frameBuffer);
		imguiWindow->start(width, height, sceneManager->getCurrentScene()->getMouseX(), sceneManager->getCurrentScene()->getMouseY());
		loop();
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
			eventSystem.handleEvents(Type::keyPressed);
			eventSystem.handleEvents(Type::keyReleased);
			glfwPollEvents();
			auto scene = sceneManager->getCurrentScene();
			//TEXTURE SELECTION UPDATES
			scene->setPicking();
			frameBuffer->bind();
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			//SCENE UPDATES
			scene->update(deltaTime);
			frameBuffer->unBind();
			//inverse for view has to be taken after render
			//IMGUI UPDATES

			imguiWindow->update(deltaTime, scene);

			//imguiwindow->render(width, height);
			glfwSwapBuffers(gameWindow->getWindow());
			gameClock->updateTime();
			deltaTime = gameClock->getTime();
			timePerSec += deltaTime;
			loopCount++;
			//std::cout << "Frame time: " << deltaTime << std::endl;
			if (timePerSec > 1) {
				//std::cout << "FPS1: " << loopCount << std::endl;
				std::cout << "FPS2: " << 1 / deltaTime << std::endl;
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

	void geProject::Application::startGamePlay(GameStartEvent* start) {
		if (start->getType() == Type::gameStart) {
			std::cout << "starting play" << std::endl;
			auto scene = sceneManager->getCurrentScene();
			scene->serialize(scene->getFilePath());
			scene->serialize("../../../../Game/assets/levels/levelEditor.json");
			scene->setPhysics(true);
			eventSystem.setContext(GameplayContext);
			scene->setActiveEntity(-1);
		}
	}

	void geProject::Application::stopGamePlay(GameStopEvent* stop) {
		if (stop->getType() == Type::gameStop) {
			std::cout << "stopping play" << std::endl;
			auto scene = sceneManager->getCurrentScene();
			scene->reloadLevel(scene->getFilePath());
			scene->setPhysics(false);
			scene = sceneManager->getCurrentScene();
			scene->deserialize(scene->getFilePath());
			eventSystem.setContext(EditorContext);
			scene->setActiveEntity(-1);
		}
	}

	void geProject::Application::loadGame(GameLoadEvent* load) {
		if (load->getType() == Type::gameLoad) {
			std::cout << "GAME LOADED" << std::endl;
			if (load->sceneId == 1) {
				sceneManager->getCurrentScene()->setActiveEntity(-1);
			}
			sceneManager->switchScene(load->sceneId);
			if (load->sceneId > 1) {
				sceneManager->getCurrentScene()->init();
			}
		}
	}



}
