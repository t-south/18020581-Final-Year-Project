#include "PlayerController.h"


geProject::PlayerController::PlayerController(/*EntityManager& emanager, Physics& pmanager,*/ Camera& camera) : /*entitymanager(emanager), physicsmanager(pmanager),*/ playerCamera(camera){
	entityId = entitymanager.getPlayerId();
	entitymanager.assignDamage(entityId, Damage());
	nextDmgType = dmgTypes::FIRE;
	eventSystem.subscribe(this, &PlayerController::rotateToCursor);
}

void geProject::PlayerController::dash(){

}

void geProject::PlayerController::attack(){
	Damage dmg = entitymanager.getDamageComponent(entityId);
	
	if (dmg.coolDown == 0){
		physicsmanager.createProjectile(entityId);
		switch (dmg.dmgType) {
		case dmgTypes::FIRE:
			dmg.coolDown = 5;
			dmg.dmgModifier = 1.2f;
			break;
		case dmgTypes::WATER:
			dmg.coolDown = 1;
			dmg.dmgModifier = 0.5f;
			break;
		case dmgTypes::EARTH:
			dmg.coolDown = 5;
			dmg.dmgModifier = 3;
			break;
		case dmgTypes::LIGHTNING:
			dmg.coolDown = 2;	
			dmg.dmgModifier = 1;
			break;
		default:
			break;
		}		
	}
	dmg.coolDown -= 1;	
	entitymanager.assignDamage(entityId, dmg);
}

void geProject::PlayerController::specialAttack()
{
}

void geProject::PlayerController::shield(){

}

void geProject::PlayerController::switchAbility(){
	switch (nextDmgType) {
	case dmgTypes::FIRE:
		entitymanager.assignDamage(entityId, Damage{ .coolDown = 0, .dmgType = dmgTypes::FIRE });
		nextDmgType = WATER;
		break;
	case dmgTypes::WATER:
		entitymanager.assignDamage(entityId, Damage{ .coolDown = 0, .dmgType = dmgTypes::WATER });
		nextDmgType = EARTH;
		break;
	case dmgTypes::EARTH:
		entitymanager.assignDamage(entityId, Damage{ .coolDown = 0, .dmgType = dmgTypes::EARTH });
		nextDmgType = LIGHTNING;
		break;
	case dmgTypes::LIGHTNING:
		entitymanager.assignDamage(entityId, Damage{ .coolDown = 0, .dmgType = dmgTypes::LIGHTNING });
		nextDmgType = FIRE;
		break;
	default:
		break;
	}
	
	
}

void geProject::PlayerController::moveTo(float x, float y, float dt){	
	float newX = x * dt;
	float newY = y * dt;
	physicsmanager.applyLinearImpulse(entityId, newX, newY);
}

void geProject::PlayerController::rotateToCursor(MouseMoveEvent* mouseMove){
	// PRE: IF EVENT IS WITHIN CONTEXT
	if (mouseMove->contextCheck(GameplayContext)) {
		//PI / 2 = 1.571 Rad
		Transform trans = entitymanager.getTransformComponent(entityId);
		float rotate = atan2((mouseMove->posY - trans.position[1]), (mouseMove->posX - trans.position[0]));	
		physicsmanager.applyRotation(entityId, rotate - 1.571);
	}
}

void geProject::PlayerController::rotateTo(float desiredDirection, float currentDirection){

}


void geProject::PlayerController::update(){	
	// PRE: IF EVENT IS WITHIN CONTEXT
	if (entityId == -1) {
		entityId = entitymanager.getPlayerId();
	}
	if (eventSystem.getContext() == GameplayContext) {
		Transform trans = entitymanager.getTransformComponent(entityId);
		playerCamera.setCentredPosition(trans.position[0], trans.position[1]);
	}
}


