#include <ge_engine/geProject.h>
#include <iostream>
class Game : public geProject::Application {
public:
	Game() {

	}
	~Game(){}
};


geProject::Application* geProject::CreateApplication() {
	return new Game;
}

int main(int argc, char** argv) {
	std::cout << "Test" << std::endl;
	Game* game =  new Game();
	game->Startup();
	return 0;
}