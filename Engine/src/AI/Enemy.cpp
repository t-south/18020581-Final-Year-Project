#include "Enemy.h"

geProject::Planner geProject::Enemy::actionPlanner;

geProject::Enemy::Enemy(){
	aiController = new EnemyController();
	idleState();
}

void geProject::Enemy::update(float deltaTime) {
	if (path.size() == 0) {
		//planPath();
	}
	if (commandQueue.size() > 0) {
		Command* command = commandQueue.front();
		aiController->update(deltaTime);
		if (command)
		{
			command->execute(*aiController);
		}
		commandQueue.pop();
	}
	RotateCommand* rotate = new RotateCommand();
	rotate->rotate = 10;
	commandQueue.push(rotate);
	
	
}

void geProject::Enemy::executeAction(){

}

geProject::Goal geProject::Enemy::chooseGoal(){	
	Goal newGoal = Goal();
	for (auto& goal : availableGoals) {
		if (goal.priority > newGoal.priority) {
			newGoal = goal;
		}
	}	
	return newGoal;
}

void geProject::Enemy::addGoal(Goal& goal){
	availableGoals.push_back(goal);
}

void geProject::Enemy::removeGoal(Goal& goal){	
	//availableGoals.erase(std::remove(availableGoals.begin(), availableGoals.end(), goal), availableGoals.end());
}

void geProject::Enemy::moveState(){
	currentState = State::MOVE;
}

void geProject::Enemy::idleState(){
	currentState = State::IDLE;
	if (actionPlan.size() > 0) {
		actionState();
	}
}

void geProject::Enemy::actionState(){
	currentState = State::ACTION;

}

float geProject::Enemy::calculateEuclidean(float originx, float originy, float destx, float desty)
{
	float x = originx - destx;
	float y = originy - desty;
	return sqrt(pow(x,2) + pow(y, 2));
}

std::vector<geProject::pathNode> geProject::Enemy::planPath(float originX, float originY, float destinationX, float destinationY){	
	std::vector<pathNode> openList;
	std::vector<pathNode> closedList;
	float currX = (int)(originX * 4);
	float currY = (int)(originY * 4);
	float destX = (int)(destinationX * 4);
	float destY = (int)(destinationY * 4);
	pathNode startNode = pathNode();
	startNode.x = currX;
	startNode.y = currY;
	openList.push_back(startNode);
	
	//A STAR
	while (openList.size() > 0) {
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
				path.push_back(expandedNode);
				expandedNode = *expandedNode.parent;
			}
			return path;
		}
		openList.erase(openList.begin() + index);
		closedList.push_back(expandedNode);
		//check all 8 adjacent squares to see if nodes are available
		
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {				
				if (worldstate.getTile(expandedNode.x + i, expandedNode.y + j) && !(i == 0 && j == 0)) {
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
