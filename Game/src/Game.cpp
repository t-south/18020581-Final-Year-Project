#include <geProject.h>
class Game : public geProject::Application {
public:
	Game() {

	}
	~Game(){}
};
int main(){
	Game* game = new Game();
	game->Startup();
	delete game;
	return 0;
}