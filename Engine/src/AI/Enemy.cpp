#include "Enemy.h"

geProject::Planner geProject::Enemy::actionPlanner;

geProject::Enemy::Enemy(int entity): entityId(entity){
	aiController = new EnemyController(entityId);
	Transform transform = entitymanager.getTransformComponent(entityId);
	position = transform.position;	
	homelocation = position;

	availableGoals.push_back(new AttackEnemyGoal());
	//availableGoals.push_back(CalmDownGoal());
	availableGoals.push_back(new PatrolGoal());
	availableGoals.push_back(new GoHomeGoal());
	availableGoals.push_back(new InvestigateGoal());
	//availableGoals.push_back(FleeGoal());
	//availableGoals.push_back(StayAliveGoal());


	actionsAvailable.push_back(new DodgeAction());
	actionsAvailable.push_back(new ShieldAction());
	actionsAvailable.push_back(new DashAction());
	actionsAvailable.push_back(new RecoverEnergyAction());
	//actionsAvailable.push_back(new PickupObjectAction());
	actionsAvailable.push_back(new InvestigateAction());
	actionsAvailable.push_back(new PatrolAction());
	actionsAvailable.push_back(new WaitAction());
	actionsAvailable.push_back(new GoHomeAction(homelocation[0], homelocation[1]));
	actionsAvailable.push_back(new FireAttackAction());
	actionsAvailable.push_back(new WaterAttackAction());
	actionsAvailable.push_back(new WindAttackAction());
	actionsAvailable.push_back(new EarthAttackAction());
	availableGoals.push_back(new RestGoal());
	entitymanager.assignAgent(entityId, Agent());
	currentGoal = availableGoals[0];
}

void geProject::Enemy::update(float deltaTime) {
	double pi = 3.14159265; 
	Agent agent = entitymanager.getAgentComponent(entityId);
	int agentStateDetails = agent.agentStateDetails;
	orientAgent();
	std::cout << agent.playerInRange << std::endl;
	if (agent.playerInRange == false) {
		std::cout << "test" << std::endl;
	}
	if (agent.playerInRange == true) {
		Transform playerpos = entitymanager.getTransformComponent(entitymanager.getPlayerId());
		if (physicsmanager.checkTargetObstructed(position[0], position[1], playerpos.position.x, playerpos.position.y)) {
			agentStateDetails &= ~ENEMY_VISIBLE;
		}
		else {
			agentStateDetails |= ENEMY_VISIBLE;
		}
	}
	
	if (energy <= 0) {
		agentStateDetails = agentStateDetails & ~HAS_ENERGY;
		entitymanager.updateAnimationState(entityId, "Sleeping");
	}
	if ((agentStateDetails & ENEMY_DEAD) == ENEMY_DEAD) {
		agentStateDetails &= ~ENEMY_DEAD;
	}
	chooseGoal(agentStateDetails);
	//std::cout << agentStateDetails << std::endl;
	//std::cout << currentGoal->name << std::endl;
	//std::cout << "agent " << entityId << ": "<< energy << std::endl;
	if (currentState == IDLE) {		
		path.clear();
		if (actionPlan.size() == 0) {
			actionPlan = actionPlanner.createPlan(*currentGoal, entityId ,actionsAvailable);
			if (actionPlan.size() > 0) {
				currentState = ACTION;
			}
		}
		else {			
			//find the path to the target of the action
			path = worldstate.planPath(position[0], position[1], actionPlan[0]->target.position.x, actionPlan[0]->target.position.y);
			if (path.size() > 0) {				
				actionPlan[0]->cost *= path.size();
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
		if (actionPlan.size() > 0) {
			actionPlan[0]->executeAction(position[0], position[1]);
			if (actionPlan[0]->attack) {
				AttackCommand* atk = new AttackCommand();		
				entitymanager.assignDamage(entityId, Damage{.dmgType = actionPlan[0]->attackType});
				commandQueue.push(atk);
				actionPlan[0]->attack == false;
				Transform trans = entitymanager.getTransformComponent(entitymanager.getPlayerId());
				desiredirection = atan2((trans.position[1] - position[1]), (trans.position[0] - position[0]));
				desiredirection -= 1.571;
				if (desiredirection < 0) {
					desiredirection += (pi * 2);
				}
				rotateAgent();			
			}
			//if action has been successfully executed remove from the plan
			if (actionPlan[0]->completed) {
				actionPlan[0]->completed = false;
				energy -= actionPlan[0]->cost;
				agentStateDetails = actionPlan[0]->setEffect(agentStateDetails);
				actionPlan.erase(actionPlan.begin());
				//set state back to idle if theres nothing left of the action plan
				if (actionPlan.size() == 0) {					
					for (auto& goal : availableGoals) {	
						if (goal == currentGoal) {
							currentGoal->decreasePriority();
						}
						else {
							goal->increasePriority();
						}						
					}
					
				}
			}
			currentState = IDLE;
		}
	}

	if (currentState == MOVE && path.size() > 0) {
		entitymanager.updateAnimationState(entityId, "Idle");
		//check if difference in direction is above or below 1 degree, then adjust the current direction to the new direction		
		if (currentdirection - desiredirection * (180 / pi) > 1 || currentdirection - desiredirection * (180 / pi) < -1) {
			rotateAgent();
		}
		else {
			//check if the agent has moved to within the desired tile range, then halt movement to agent
			if (path[0].x - position[0] < 0.2f && path[0].y - position[1] < 0.2f && path[0].x - position[0] > -0.2f && path[0].y - position[1] > -0.2f) {
				path.erase(path.begin());
				if (path.size() == 0 && actionPlan.size() == 0) {
					currentState = IDLE;
				}
				else if (path.size() == 0 && actionPlan.size() > 0) {
					actionPlan[0]->inRange = true;
					currentState = ACTION;
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
				if (worldstate.calculateEuclidean(position[0], position[1], path[0].x, path[0].y) > 0.6f) { 
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
	entitymanager.updateAgentState(entityId, agentStateDetails);
}

void geProject::Enemy::executeAction(){

}

void geProject::Enemy::chooseGoal(int agentState){	
	updateActionCost(agentState);
	if (!currentGoal->checkValid(agentState)) {
		currentGoal = nullptr;		
	}
	for (auto& goal : availableGoals) {		
		if (agentState & ~HAS_ENERGY) {			
			if (goal->goalType == COMBAT) {
				goal->priority = 10;

			}
			else {
				goal->priority = goal->priority -1;
			}
		}
		if (goal->checkValid(agentState)) {
			if (currentGoal == nullptr) {
				currentGoal = goal;
			}
			else if (goal->priority > currentGoal->priority) {
				currentGoal = goal;
				currentState = IDLE;
				path.clear();
				actionPlan.clear();
			}
		}		
	}	
}

void geProject::Enemy::addGoal(Goal* goal){
	availableGoals.push_back(goal);
}

void geProject::Enemy::removeGoal(Goal* goal){	
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



void geProject::Enemy::rotateAgent(){	
	RotateCommand* rotate = new RotateCommand();
	rotate->desiredDirection = desiredirection;
	rotate->currentDirection = currentdirection;
	commandQueue.push(rotate);
}




std::vector<geProject::pathNode> geProject::Enemy::getPath(){return path;}

int geProject::Enemy::getPathSize(){return path.size();}

int geProject::Enemy::getEnemyId() {return entityId;}

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

void geProject::Enemy::updateActionCost(int agentState){
	for (auto& action : actionsAvailable) {
		action->cost = 1;
		if (path.size() > 0 && actionPlan.size() > 0) {
			actionPlan[0]->cost *= path.size();
		}
		if ((agentState & ENEMY_VISIBLE) == agentState) {
			if (action->actiontype != COMBAT) {
				action->cost *= 4;
			}
		}
		if ((agentState & AGENT_ANGRY) == agentState) {
			if (action->actiontype == COMBAT) {
				action->cost = 1;
			}
			else if (action->actiontype == DUTY) {
				action->cost *= 2;
			}
		}
		if ((agentState & ~HAS_ENERGY) == agentState) {
			action->cost *= std::rand() % 2;
		}
	}
}





