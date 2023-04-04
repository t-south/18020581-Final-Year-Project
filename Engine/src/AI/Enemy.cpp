#include "Enemy.h"

geProject::Planner geProject::Enemy::actionPlanner;

geProject::Enemy::Enemy(){
	idleState();
}

void geProject::Enemy::update(float deltaTime){
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
	availableGoals.erase(goal);
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
