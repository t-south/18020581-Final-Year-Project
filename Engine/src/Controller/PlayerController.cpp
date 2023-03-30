#include "PlayerController.h"

geProject::PlayerController::PlayerController(EntityManager& manager, int entity): eManager(manager), entityId(entity){}

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

void geProject::PlayerController::moveTo(){
	Transform* trans = eManager.getTransformComponent(entityId);
	Controls* control = eManager.getControllerComponent(entityId);
	trans->position[0] += control->runspeed;
	trans->position[1] += control->runspeed;
	eManager.assignTransform(trans);
}
