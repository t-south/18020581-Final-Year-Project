#include "Application.h"


geProject::EventHandler eventSystem;
geProject::ResourceManager resourcemanager;
geProject::WorldState worldstate;
geProject::EntityManager entitymanager;
geProject::Physics physicsmanager;

namespace geProject {
	Application::Application() {}
	Application::~Application() {}

	void Application::Startup() {
		eventSystem = EventHandler();
		eventSystem.subscribe(this, &Application::startGamePlay);
		eventSystem.subscribe(this, &Application::stopGamePlay);
		eventSystem.subscribe(this, &Application::loadGame);
		gameWindow = new geProject::Window("game", 1920, 1080);
		gameClock = Clock::getInstance();
		sceneManager = new SceneStates();
		std::shared_ptr<Scene> menuScene = std::make_shared<MenuScene>();
		std::shared_ptr<Scene> levelEditorScene = std::make_shared<LevelEditorScene>();
		std::shared_ptr<Scene> levelScene = std::make_shared<LevelScene>();
		int menuSceneId = sceneManager->addScene(menuScene);
		int levelSceneId = sceneManager->addScene(levelScene);
		int editorSceneId = sceneManager->addScene(levelEditorScene);
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
		while (running &&  !glfwWindowShouldClose(gameWindow->getWindow())) {
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
			glfwSwapBuffers(gameWindow->getWindow());           
			
			//imguiwindow->render(width, height);
			gameClock->updateTime();
			deltaTime = gameClock->getTime();
			timePerSec += deltaTime;
			loopCount++;	
			if (timePerSec > 1) {	
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

	void geProject::Application::startGamePlay(GameStartEvent* start) {
		if (start->getType() == Type::gameStart) {
	
			auto scene = sceneManager->getCurrentScene();
			scene->serialize(scene->getFilePath());
			scene->serialize("Engine/Game/assets/levels/levelEditor.json");
			scene->setPhysics(true);
			eventSystem.setContext(GameplayContext);
			scene->setActiveEntity(-1);
		}
	}

	void geProject::Application::stopGamePlay(GameStopEvent* stop) {
		if (stop->getType() == Type::gameStop) {	
			auto scene = sceneManager->getCurrentScene();
			scene->reloadLevel();
			scene->setPhysics(false);
			scene = sceneManager->getCurrentScene();
			scene->deserialize(scene->getFilePath());			
			eventSystem.setContext(EditorContext);
			scene->setActiveEntity(-1);
			scene->init();
		}
	}

	void geProject::Application::loadGame(GameLoadEvent* load) {
		if (load->getType() == Type::gameLoad) {	
			if (load->sceneId == -1) {
				running = false;
			}
			else {
				sceneManager->switchScene(load->sceneId);
				sceneManager->getCurrentScene()->reloadLevel();
				sceneManager->getCurrentScene()->setGoapStatus(load->aiActive);
				sceneManager->getCurrentScene()->init();
				sceneManager->getCurrentScene()->setActiveEntity(-1);
			}

		}
	}



}
int main(int argc, char** argv) {
	geProject::Application* game = new geProject::Application();
	game->Startup();
	return 0;
}