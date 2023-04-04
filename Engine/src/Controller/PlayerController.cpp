#include "PlayerController.h"


geProject::PlayerController::PlayerController(EntityManager& manager, Camera& camera, Physics& pManager,  int entity): eManager(manager), playerCamera(camera), physicsManager(pManager),  entityId(entity){
	eventSystem.subscribe(this, &PlayerController::rotateToCursor);
}

void geProject::PlayerController::dash(){

}

void geProject::PlayerController::attack(){

}

void geProject::PlayerController::specialAttack()
{
}

void geProject::PlayerController::shield(){

}

void geProject::PlayerController::switchAbility()
{
}

void geProject::PlayerController::moveTo(float x, float y){	
	/*Transform* trans = eManager.getTransformComponent(entityId);
	Controls* control = eManager.getControllerComponent(entityId);

	trans->position[0] += newX;
	trans->position[1] += newY;
	*/
	float newX = x * dt;
	float newY = y * dt;
	physicsManager.applyLinearImpulse(entityId, newX, newY);
	//playerCamera.setPosition(glm::vec2(trans->position[0], trans->position[1]));
	//eManager.assignTransform(entityId, *trans);	
}

void geProject::PlayerController::rotateToCursor(MouseMoveEvent* mouseMove){
	if (mouseMove->contextCheck(GameplayContext)) {
		//PI / 2 = 1.571 Rad
		Transform* trans = eManager.getTransformComponent(entityId);
		float rotate = atan2((mouseMove->posY - trans->position[1]), (mouseMove->posX - trans->position[0]));		
		physicsManager.applyRotation(entityId, rotate - 1.571);

	}
}


void geProject::PlayerController::update(float deltaTime){
	dt = deltaTime;
	if (entityId == -1) {
		entityId = eManager.getPlayerId();
	}
	if (eventSystem.getContext() == GameplayContext) {
		Transform* trans = eManager.getTransformComponent(entityId);
		playerCamera.setCentredPosition(trans->position[0], trans->position[1]);
	}
}


