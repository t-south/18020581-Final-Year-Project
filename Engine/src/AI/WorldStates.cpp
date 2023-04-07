#include "WorldStates.h"


unsigned int geProject::WorldState::getWorldState() { return worldState; }
void geProject::WorldState::updateWorldState(unsigned int newState){worldState = worldState & newState;}

void geProject::WorldState::testWorldMap()
{
	for (int i = 0; i < worldWidth; i++) {
		for (int j = 0; j < worldHeight; j++) {
			if (worldMap[i][j] == 1) {
				std::cout << ".";
			}
			else {
				std::cout << "=";
			}
		}
	std::cout << " |||| " << std::endl;
	}
}

bool geProject::WorldState::getTile(int x, int y)
{
	if (x < 0 || y < 0 ) {
		return false;
	}

	return worldMap[x][y];
}

void geProject::WorldState::addToMap(int x, int y, int accessible) {
	worldMap[x][y] = accessible;

}

void geProject::WorldState::generateWorldMap(int width, int height){
	worldWidth = width + 2;
	worldHeight = height + 2;
	if (worldMap.size() > 0) {
		worldMap.clear();
	}
	for (int i = 0; i < worldWidth; i++) {
		worldMap.push_back(std::vector<int>());
		for (int j = 0; j < worldHeight; j++) {
			worldMap[i].push_back(1);			
		}
	
	}
}




