#include "Enemy.h"

geProject::Planner geProject::Enemy::actionPlanner;

geProject::Enemy::Enemy(int entity): entityId(entity){
	aiController = new EnemyController(entityId);
	Transform transform = entitymanager.getTransformComponent(entityId);
	position = transform.position;	
	//availableGoals.push_back(AttackEnemyGoal());
	//availableGoals.push_back(CalmDownGoal());
	availableGoals.push_back(PatrolGoal());
	actionsAvailable.push_back(new DodgeAction());
	actionsAvailable.push_back(new ShieldAction());
	actionsAvailable.push_back(new DashAction());
	actionsAvailable.push_back(new RecoverEnergyAction());
	actionsAvailable.push_back(new PickupObjectAction());
	actionsAvailable.push_back(new InvestigateAction());
	actionsAvailable.push_back(new PatrolAction());
	actionsAvailable.push_back(new WaitAction());
	actionsAvailable.push_back(new GoHomeAction());
	actionsAvailable.push_back(new FireAttackAction());
	actionsAvailable.push_back(new WaterAttackAction());
	actionsAvailable.push_back(new WindAttackAction());
	actionsAvailable.push_back(new EarthAttackAction());
	//availableGoals.push_back(RestGoal());
	//availableGoals.push_back(InvestigateGoal());
	//availableGoals.push_back(FleeGoal());
	//availableGoals.push_back(StayAliveGoal());
	chooseGoal();
}

void geProject::Enemy::update(float deltaTime) {
	double pi = 3.14159265;
	orientAgent();

	if (currentState == IDLE) {		
		path.clear();
		if (actionPlan.size() == 0) {
			actionPlan = actionPlanner.createPlan(currentGoal, agentStateDetails, actionsAvailable);
		}
		else if (actionPlan.size() > 0) {
			currentState = ACTION;
		}
		else {
			Transform playerpos = entitymanager.getTransformComponent(entitymanager.getPlayerId());
			planPath(position[0], position[1], playerpos.position.x, playerpos.position.y);
			if (path.size() > 0) {
				desiredirection = atan2((path[0].y - position[1]), (path[0].x - position[0]));
				desiredirection -= 1.571;
				if (desiredirection < 0) {
					desiredirection += (pi * 2);
				}
				currentState = MOVE;
			}
		}
	}	

	if (currentState == ACTION) {

	}

	if (currentState == MOVE && path.size() > 0) {
		//check if difference in direction is above or below 1 degree, then adjust the current direction to the new direction		
		if (currentdirection - desiredirection * (180 / pi) > 1 || currentdirection - desiredirection * (180 / pi) < -1) {
			rotateAgent();
		}
		else {
			//check if the agent has moved to within the desired tile range, then halt movement to agent
			if (path[0].x - position[0] < 0.2f && path[0].y - position[1] < 0.2f && path[0].x - position[0] > -0.2f && path[0].y - position[1] > -0.2f) {
				path.erase(path.begin());
				if (path.size() == 0) {
					currentState = IDLE;
				}
				else{
					desiredirection = atan2((path[0].y - position[1]), (path[0].x - position[0]));
					desiredirection -= 1.571;
					if (desiredirection < 0){desiredirection += (pi * 2);}
					moveAgent(0, 0, deltaTime);
				}
			}
			else {
				//check that the distance is not too far from next place on the path, if so set the agent back to the idle state, otherwise move it to the next position
				if (calculateEuclidean(position[0], position[1], path[0].x, path[0].y) > 0.6f) { 
					currentState = IDLE; 
				}
				else {
					moveAgent(position.x - path[0].x, position.y - path[0].y, deltaTime);
				}
			}
		}
		
	}

	//go through all commands added to the queue and execute for this frame
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

void geProject::Enemy::chooseGoal(){	
	currentGoal = availableGoals[0];
	for (auto& goal : availableGoals) {
		if (goal.priority > currentGoal.priority) {
			currentGoal = goal;
		}
	}	
	
}

void geProject::Enemy::addGoal(Goal& goal){
	availableGoals.push_back(goal);
}

void geProject::Enemy::removeGoal(Goal& goal){	
	//availableGoals.erase(std::remove(availableGoals.begin(), availableGoals.end(), goal), availableGoals.end());
}




void geProject::Enemy::setDesiredDirection(){

}





void geProject::Enemy::moveAgent(float x, float y, float dt){
	if (currentState == MOVE) {
		MoveCommand* move = new MoveCommand();
		move->x = x;
		move->y = y;
		move->dt = dt;
		commandQueue.push(move);
	}
}

float geProject::Enemy::calculateEuclidean(float originx, float originy, float destx, float desty){
	float x = originx - destx;
	float y = originy - desty;
	return sqrt(pow(x,2) + pow(y, 2));
}

void geProject::Enemy::rotateAgent(){	
	RotateCommand* rotate = new RotateCommand();
	rotate->desiredDirection = desiredirection;
	rotate->currentDirection = currentdirection;
	commandQueue.push(rotate);
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

std::vector<geProject::pathNode> geProject::Enemy::getPath()
{
	return path;
}

int geProject::Enemy::getPathSize()
{
	return path.size();
}

int geProject::Enemy::getEnemyId() {
	return entityId;
}

void geProject::Enemy::orientAgent() {
	Transform transform = entitymanager.getTransformComponent(entityId);
	currentdirection = transform.rotation;
	//currentdirection = (int)std::round(transform.rotation) % 360;
	currentdirection = fmod(currentdirection, 360);
	if (currentdirection < 0) {
		currentdirection += 360;
	}
	oldposition = position;
	position = transform.position;
}
