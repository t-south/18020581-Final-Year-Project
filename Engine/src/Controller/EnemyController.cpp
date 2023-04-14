#include "EnemyController.h"

geProject::EnemyController::EnemyController(int entity): entityId(entity){
	desiredVelocity = glm::vec3();
	currentVelocity = glm::vec3();
	steeringVelocity = glm::vec3();
	entitymanager.assignDamage(entityId, Damage());
}

void geProject::EnemyController::dash(){

}

void geProject::EnemyController::attack(){
	Damage dmg = entitymanager.getDamageComponent(entityId);
	dmg.dmgType = 2;
	physicsmanager.createProjectile(entityId);
	entitymanager.assignDamage(entityId, dmg);
}

void geProject::EnemyController::specialAttack(){

}

void geProject::EnemyController::shield(){

}

void geProject::EnemyController::switchAbility(){

}

void geProject::EnemyController::moveTo(float x, float y, float dt){
	if (x == 0 && y == 0) {
		physicsmanager.applyLinearImpulse(entityId, x, y);
	}
	else {
		desiredVelocity = glm::normalize(glm::vec3(x, y, 0)) * maxVelocity;
		steeringVelocity = desiredVelocity - currentVelocity;
		currentVelocity += steeringVelocity;
		currentVelocity *= dt;
		physicsmanager.applyLinearImpulse(entityId, -currentVelocity[0], -currentVelocity[1]);
	}
}

void geProject::EnemyController::rotateTo(float desiredDirection, float currentDirection){
	double pi = 3.14159265;
	currentDirection = currentDirection * (pi / 180);
	float rotation = desiredDirection - currentDirection;
	//Transform* trans = entitymanager.getTransformComponent(entityId);	
	while (rotation < -180 * (pi / 180)) rotation += 360 * (pi / 180);
	while (rotation > 180 * (pi / 180)) rotation -= 360 * (pi / 180);
	float anglechange = 0;
	if (rotation < 0.1f || rotation > -0.1f) {
		anglechange = 2 * (pi / 180);
	}
	else {
		anglechange = 1 * (pi / 180);
	}
	float newangle = currentDirection + std::min(anglechange, std::max(-anglechange, rotation));
	
	physicsmanager.applyRotation(entityId, newangle);
	//std::cout << rotate << std::endl;
}

void geProject::EnemyController::update(float deltaTime){

}
