#include "PlayerController.h"


geProject::PlayerController::PlayerController(/*EntityManager& emanager, Physics& pmanager,*/ Camera& camera) : /*entitymanager(emanager), physicsmanager(pmanager),*/ playerCamera(camera){
	dt = 0;
	entityId = entitymanager.getPlayerId();
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
	float newX = x * dt;
	float newY = y * dt;
	physicsmanager.applyLinearImpulse(entityId, newX, newY);
}

void geProject::PlayerController::rotateToCursor(MouseMoveEvent* mouseMove){
	if (mouseMove->contextCheck(GameplayContext)) {
		//PI / 2 = 1.571 Rad
		Transform trans = entitymanager.getTransformComponent(entityId);
		float rotate = atan2((mouseMove->posY - trans.position[1]), (mouseMove->posX - trans.position[0]));		
		physicsmanager.applyRotation(entityId, rotate - 1.571);

	}
}

void geProject::PlayerController::rotateTo(float rotate)
{
}


void geProject::PlayerController::update(float deltaTime){
	dt = deltaTime;
	if (entityId == -1) {
		entityId = entitymanager.getPlayerId();
	}
	if (eventSystem.getContext() == GameplayContext) {
		Transform trans = entitymanager.getTransformComponent(entityId);
		playerCamera.setCentredPosition(trans.position[0], trans.position[1]);
	}
}


