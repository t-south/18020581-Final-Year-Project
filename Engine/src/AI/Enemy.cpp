#include "Enemy.h"

geProject::Planner geProject::Enemy::actionPlanner;

geProject::Enemy::Enemy(int entity, bool goap): entityId(entity), aiActive(goap){
	aiController = new EnemyController(entityId);
	Transform transform = entitymanager.getTransformComponent(entityId);
	position = transform.position;	
	homelocation = position;
	if (aiActive) {
		availableGoals.push_back(new AttackEnemyGoal());
		//availableGoals.push_back(CalmDownGoal());
		availableGoals.push_back(new PatrolGoal());
		availableGoals.push_back(new GoHomeGoal());
		availableGoals.push_back(new InvestigateGoal());
		availableGoals.push_back(new RestGoal());
		//availableGoals.push_back(FleeGoal());
		//availableGoals.push_back(StayAliveGoal());
		actionsAvailable.push_back(new DodgeAction(entityId));
		actionsAvailable.push_back(new RecoverEnergyAction(entityId));
		//actionsAvailable.push_back(new PickupObjectAction());
		actionsAvailable.push_back(new InvestigateAction(entityId));
		actionsAvailable.push_back(new PatrolAction(entityId));
		actionsAvailable.push_back(new WaitAction(entityId));
		actionsAvailable.push_back(new GoHomeAction(homelocation[0], homelocation[1], entityId));
		actionsAvailable.push_back(new FireAttackAction(entityId));
		actionsAvailable.push_back(new WaterAttackAction(entityId));
		actionsAvailable.push_back(new WindAttackAction(entityId));
		actionsAvailable.push_back(new EarthAttackAction(entityId));
		entitymanager.assignHealth(entityId, Health());
		entitymanager.assignDamage(entityId, Damage());
		entitymanager.assignAgent(entityId, Agent());
		currentGoal = availableGoals[0];
	}
}

void geProject::Enemy::update(float deltaTime) {
	double pi = 3.14159265; 
	orientAgent();
		

	if (aiActive) {	
		updateAgentVisibility();
		Agent agent = entitymanager.getAgentComponent(entityId);
		if (agent.energy <= 10 && (agent.agentStateDetails & ALERT) != ALERT && (agent.agentStateDetails & ENEMY_VISIBLE) != ENEMY_VISIBLE) {
			agent.agentStateDetails = agent.agentStateDetails & ~HAS_ENERGY;
			entitymanager.updateAnimationState(entityId, "Sleeping");
		}
		if ((agent.agentStateDetails & ALERT) == ALERT && (agent.agentStateDetails & ENEMY_VISIBLE) != ENEMY_VISIBLE) {
			Transform playerpos = entitymanager.getTransformComponent(entitymanager.getPlayerId());
			agent.anomalypos = playerpos.position;
			entitymanager.updateAnimationState(entityId, "Investigating");
		}
		if (((agent.agentStateDetails & ENEMY_VISIBLE) == ENEMY_VISIBLE)) {
			agent.agentStateDetails &= ~ALERT;
			entitymanager.updateAnimationState(entityId, "Attacking");
		}
		if (agent.alertLevel == 0 && (agent.agentStateDetails & INVESTIGATED) == INVESTIGATED) {
			agent.agentStateDetails &= ~ALERT & ~INVESTIGATED;
		}
		if ((agent.agentStateDetails & ENEMY_DEAD) == ENEMY_DEAD) {
			agent.agentStateDetails &= ~ENEMY_DEAD;
		}
		if ((agent.agentStateDetails & COOLDOWN) != COOLDOWN) {
			cooldown--;
			if (cooldown <= 0) {
				agent.agentStateDetails |= COOLDOWN;
				cooldown = 5;
			}
		}
		entitymanager.assignAgent(entityId, agent);
		chooseGoal(agent.agentStateDetails);
	}

	Agent agent = entitymanager.getAgentComponent(entityId);
	if (currentState == IDLE) {	
		if (aiActive) {
			path.clear();
			actionPlan.clear();
			if (actionPlan.size() == 0 && currentGoal != nullptr) {
				actionPlan = actionPlanner.createPlan(*currentGoal, agent.agentStateDetails, entityId, actionsAvailable);
				if (actionPlan.size() > 0) {
					currentState = ACTION;
				}
			}
		}
		else {
			currentState = MOVE;
		}
	}	

	if (currentState == ACTION) {
		if (!aiActive) {
			cooldown--;
			if (agent.playerInRange && cooldown == 0) {				
				AttackCommand* atk = new AttackCommand();
				entitymanager.assignDamage(entityId, Damage());
				commandQueue.push(atk);
				Transform trans = entitymanager.getTransformComponent(entitymanager.getPlayerId());
				desiredirection = atan2((trans.position[1] - position[1]), (trans.position[0] - position[0]));
				desiredirection -= 1.571;
				if (desiredirection < 0) {
					desiredirection += (pi * 2);
				}
				cooldown = 10;			
			}
			currentState = MOVE;
			
		}
		else{
			if (actionPlan.size() == 0) {
				currentState = IDLE;
			}
			else {
				actionPlan[0]->executeAction(position[0], position[1]);
				if (path.size() == 0 && actionPlan[0]->completed == false) {
					path = worldstate.planPath(position[0], position[1], actionPlan[0]->target.position.x, actionPlan[0]->target.position.y);
				}
				if (path.size() > 0) {
					entitymanager.updateAnimationState(entityId, "Idle");
					desiredirection = atan2((path[0].y - position[1]), (path[0].x - position[0]));
					desiredirection -= 1.571;
					if (desiredirection < 0) {
						desiredirection += (pi * 2);
					}
					currentState = MOVE;
				}
				else if (actionPlan[0]->inRange) {
					if (actionPlan[0]->attack) {
						AttackCommand* atk = new AttackCommand();
						entitymanager.assignDamage(entityId, Damage{ .dmgModifier = actionPlan[0]->dmgModifier, .dmgType = actionPlan[0]->attackType });
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
				}
				if (actionPlan[0]->completed) {
					actionPlan[0]->completed = false;
					actionPlan.erase(actionPlan.begin());
					//set state back to idle if theres nothing left of the action plan
					if (actionPlan.size() == 0) {
						int prioritymod = 1;
						for (auto& goal : availableGoals) {
							if (goal == currentGoal) {
								currentGoal->decreasePriority(prioritymod);
							}
							else {
								goal->increasePriority(prioritymod);
							}
							if (agent.energy < 10 && goal->goalType != COMBAT) {
								goal->decreasePriority(prioritymod * 3);
							}
						}
					}
				}
			}
		}
	}
	if (currentState == MOVE && !aiActive) {
		entitymanager.updateAnimationState(entityId, "Idle");
		Transform trans = entitymanager.getTransformComponent(entitymanager.getPlayerId());
		desiredirection = atan2((trans.position[1] - position[1]), (trans.position[0] - position[0]));
		desiredirection -= 1.571;
		if (desiredirection < 0) {
			desiredirection += (pi * 2);
		}
		if (agent.playerInRange) {
			currentState = ACTION;
		}
		else {
			if (position != homelocation) {
				path = worldstate.planPath(position[0], position[1], homelocation.x, homelocation.y);
			}
			if (path.size() > 0 && path[0].x - position[0] < 0.2f && path[0].y - position[1] < 0.2f && path[0].x - position[0] > -0.2f && path[0].y - position[1] > -0.2f) {
				path.erase(path.begin());
			}
			else if (path.size() > 0) {
				moveAgent(position.x - path[0].x, position.y - path[0].y, deltaTime);
			}
		}


		rotateAgent();
	}

	if (currentState == MOVE && aiActive && path.size() > 0 && actionPlan.size() > 0) {
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
}


void geProject::Enemy::chooseGoal(int agentState){	
	updateActionCost(agentState);
	if (currentGoal != nullptr && !currentGoal->checkValid(agentState)) {
		currentGoal = nullptr;		
		path.clear();
		actionPlan.clear();
		currentState = IDLE;
	}
	for (auto& goal : availableGoals) {		
		if ((agentState & ~HAS_ENERGY) == agentState) {			
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
		if ((agentState & ENEMY_VISIBLE) == ENEMY_VISIBLE) {
			if (action->actiontype != COMBAT) {
				action->cost *= 4;
			}
		}
		if ((agentState & AGENT_ANGRY) == AGENT_ANGRY) {
			if (action->actiontype == COMBAT) {
				action->cost = 1;
			}

		}

	}
}

void geProject::Enemy::updateAgentVisibility(){
	Agent agent = entitymanager.getAgentComponent(entityId);
	Transform playerpos = entitymanager.getTransformComponent(entitymanager.getPlayerId());
	if (agent.playerInRange == true) {
		if (physicsmanager.checkTargetObstructed(position[0], position[1], playerpos.position.x, playerpos.position.y)) {
			if ((agent.agentStateDetails & ENEMY_VISIBLE) == ENEMY_VISIBLE) {					
				agent.alertLevel = 10;
				agent.agentStateDetails |= ALERT;

			}
			agent.agentStateDetails &= ~ENEMY_VISIBLE;

		}
		else {
			agent.agentStateDetails |= ENEMY_VISIBLE;
		}
	}
	else {
		if ((agent.agentStateDetails & ENEMY_VISIBLE) == ENEMY_VISIBLE) {		
			agent.alertLevel = 10;
			agent.agentStateDetails |= ALERT;
		}
		agent.agentStateDetails &= ~ENEMY_VISIBLE;
	}
	entitymanager.assignAgent(entityId, agent);


}





