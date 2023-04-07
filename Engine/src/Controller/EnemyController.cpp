#include "EnemyController.h"

geProject::EnemyController::EnemyController(int entity): entityId(entity){}

void geProject::EnemyController::dash(){

}

void geProject::EnemyController::attack(){

}

void geProject::EnemyController::specialAttack(){

}

void geProject::EnemyController::shield(){

}

void geProject::EnemyController::switchAbility(){

}

void geProject::EnemyController::moveTo(float x, float y){
	float newX = x;
	float newY = y;
	physicsmanager.applyLinearImpulse(entityId, newX, newY);
}

void geProject::EnemyController::rotateTo(float rotate){
	//Transform* trans = entitymanager.getTransformComponent(entityId);	
	physicsmanager.applyRotation(entityId, rotate);
	//std::cout << rotate << std::endl;
}

void geProject::EnemyController::update(float deltaTime){

}
