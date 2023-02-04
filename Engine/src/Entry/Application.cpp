#include "Application.h"


namespace geProject {
	Application::Application() {

	}
	Application::~Application() {

	}
	void Application::Startup() {
		std::cout << "Engine side test" << std::endl;		
		geProject::Window* game_window = new geProject::Window("game", 1920, 1080);
		game_window->loop();       
       
	}
}