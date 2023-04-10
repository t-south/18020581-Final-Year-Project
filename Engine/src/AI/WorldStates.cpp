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

bool geProject::WorldState::getTile(float x, float y)
{

	if (x < 0 || y < 0  || y >= worldHeight || x >= worldWidth) {
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


std::vector<geProject::pathNode> geProject::WorldState::planPath(float originX, float originY, float destinationX, float destinationY) {
	std::vector<pathNode> path;
	std::vector<pathNode> openList;
	std::vector<pathNode> closedList;
	float currX = std::round(originX * 4);
	float currY = std::round(originY * 4);
	float destX = std::round(destinationX * 4);
	float destY = std::round(destinationY * 4);
	pathNode startNode = pathNode();
	startNode.x = currX;
	startNode.y = currY;
	openList.push_back(startNode);
	//A STAR
	int track = 0;
	while (openList.size() > 0) {
		track++;
		int index = 0;
		int count = 0;
		pathNode expandedNode = openList[0];
		for (auto& node : openList) {
			if (node.fValue < expandedNode.fValue) {
				expandedNode = node;
				index = count;
			}
			count++;
		}

		if (expandedNode.x == destX && expandedNode.y == destY) {

			while (expandedNode.parent != nullptr) {
				expandedNode.x = expandedNode.x / 4;
				expandedNode.y = expandedNode.y / 4;
				path.insert(path.begin(), expandedNode);
				expandedNode = *expandedNode.parent;
			}
			std::cout  << "A* " << track << std::endl;
			return path;
		}
		openList.erase(openList.begin() + index);
		closedList.push_back(expandedNode);
		//check all 8 adjacent squares to see if nodes are available

		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (getTile(expandedNode.x + i, expandedNode.y + j) && !(i == 0 && j == 0)) {
					pathNode neighbourNode = pathNode();
					neighbourNode.x = expandedNode.x + i;
					neighbourNode.y = expandedNode.y + j;
					pathNode* newNode = new pathNode();
					*newNode = expandedNode;
					neighbourNode.parent = newNode;
					expandedNode.neighbours.push_back(neighbourNode);
				}
			}
		}

		for (auto& neighbour : expandedNode.neighbours) {
			float gValue = expandedNode.gValue + 1;
			float heuristic = calculateEuclidean(expandedNode.x, expandedNode.y, destX, destY);
			float fValue = gValue + heuristic;
			bool neighbourclosed = std::find(closedList.begin(), closedList.end(), neighbour) != closedList.end();
			bool neighbouropen = std::find(openList.begin(), openList.end(), neighbour) != openList.end();
			if (fValue < expandedNode.fValue && neighbourclosed) {
				neighbour.fValue = fValue;
				neighbour.gValue = gValue;
				expandedNode = *(neighbour.parent);
			}
			else if (expandedNode.fValue < fValue && neighbouropen) {
				neighbour.fValue = fValue;
				neighbour.gValue = gValue;
				neighbour.parent = &expandedNode;
			}
			else if (!neighbouropen && !neighbourclosed) {
				neighbour.fValue = fValue;
				neighbour.gValue = gValue;
				openList.push_back(neighbour);
			}

		}

	}


	return path;
}

float geProject::WorldState::calculateEuclidean(float originx, float originy, float destx, float desty) {
	float x = originx - destx;
	float y = originy - desty;
	return sqrt(pow(x, 2) + pow(y, 2));
}

geProject::Transform geProject::WorldState::getRandomPoint(float posx, float posy){
	float randnum = (rand() % 13 + 5) / 4.0f;
	int randdir = rand() % 8;
	posx = round(posx * 4) / 4;
	posy = round(posy * 4) / 4;
	/*
	---|-----|---
	 7 |  0  | 1
	---|-----|---
	 6 | pos | 2
	---|-----|---
	 5 |  4  | 3
	---|-----|---
	*/
	float newPosX = 0;
	float newPosY = 0;
	bool tileFound = false;
	while (!tileFound) {
		
		switch (randdir) {
		case 0:
			newPosX = posx;
			newPosY = posy + randnum;
			break;
		case 1:
			newPosX = posx + randnum;
			newPosY = posy + randnum;
			break;
		case 2:
			newPosX = posx + randnum;
			newPosY = posy;
			break;
		case 3:
			newPosX = posx + randnum;
			newPosY = posy - randnum;
			break;
		case 4:
			newPosX = posx;
			newPosY = posy - randnum;
			break;
		case 5:
			newPosX = posx - randnum;
			newPosY = posy - randnum;
			break;
		case 6:
			newPosX = posx - randnum;
			newPosY = posy;
			break;
		case 7:
			newPosX = posx - randnum;
			newPosY = posy + randnum;
			break;
		}
		tileFound = getTile(newPosX * 4, newPosY * 4);
		if (randnum > 5) {
			randnum--;
		}
		else {
			//wrap around direction
			randdir = (randdir + 1) % 8;
		}
	}
	std::cout << "X: " << newPosX <<  " Y: " << newPosY << std::endl;
	return Transform{.id = 1, .position = glm::vec2(newPosX, newPosY)};
}




