#include "Enemy.h"

geProject::Planner geProject::Enemy::actionPlanner;

geProject::Enemy::Enemy(int entity): entityId(entity){
	aiController = new EnemyController(entityId);
	Transform transform = entitymanager.getTransformComponent(entityId);
	position = transform.position;	
	idleState();
}

void geProject::Enemy::update(float deltaTime) {
	double pi = 3.14159265;
	currentdirection = entitymanager.getTransformComponent(entityId).rotation;
	if (currentdirection < 0) {
		currentdirection = 359.0f;
	}
	if (path.size() == 0) {
		planPath(position[0], position[1], 3.5f, 1.25f);
		desiredirection = atan2((path[0].y - position[1]), (path[0].x - position[0]));
		if (desiredirection < 0) // we don't want negative angles
		{
			desiredirection += (pi * 2 );
			// make negative angles positive by adding 360 degrees
		}
		desiredirection -= 1.571;
	}
	float testangle = atan2(position[1], position[0]) - atan2(path[0].y, path[0].x);
	/*
	else {
		if (path[0].x != position[0] && path[0].y != position[1]) {
			//MoveCommand* move = new MoveCommand();
			//move->x = position[0] - path[0].x;
			//move->y = position[1] - path[0].y;
			//commandQueue.push(move);
		}
		else {
			path.erase(path.begin());
		}

		
	}
	*/
	float currentDirRad = currentdirection * (pi / 180);
	//check if range of direction is above or below 1 degree, then adjust the current direction to the new direction
	if (currentdirection - desiredirection * (180 / pi) > 1 || currentdirection - desiredirection * (180 / pi) < -1) {
		RotateCommand* rotate = new RotateCommand();
		float rotation = desiredirection - currentDirRad;
		while (rotation < -180 * (pi / 180)) rotation += 360 * (pi / 180);
		while (rotation > 180 * (pi / 180)) rotation -= 360 * (pi / 180);
		float anglechange = 1 * (pi / 180);
		float testagain = std::min(anglechange, std::max(-anglechange, rotation));
		float newangle = currentDirRad + testagain;
		rotate->rotate = newangle;
		commandQueue.push(rotate);		
	}
	else {
		std::cout << "completed" << std::endl;
	}

	for (int i = 0; i < commandQueue.size(); i++) {
		Command* command = commandQueue.front();
		aiController->update(deltaTime);
		if (command)
		{
			command->execute(*aiController);
		}
		commandQueue.pop();
	}

	

	
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
	float currX = std::round(originX * 4);
	float currY = std::round(originY * 4);
	float destX = std::round(destinationX * 4);
	float destY = std::round(destinationY * 4);
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
				expandedNode.x = expandedNode.x / 4;
				expandedNode.y = expandedNode.y / 4;
				path.insert(path.begin(), expandedNode);				
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
