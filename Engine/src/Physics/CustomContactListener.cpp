#include "CustomContactListener.h"


geProject::CustomContactListener::CustomContactListener(){
	
}

geProject::CustomContactListener::~CustomContactListener(){

}


void geProject::CustomContactListener::BeginContact(b2Contact* contact) {	
	bool obstructed = false;
	bool deadly = false;
	Entity* firstData = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	Entity* secondData = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	
	if ((firstData->type & completion )== completion) {
		if ((secondData->type & player) == player || (secondData->type & playerprojectile)== playerprojectile) {
			eventSystem.publish(new GameLoadEvent(GameplayContext | ImGuiContext, 1, true));
		}
	}
	switch (firstData->type) {
	case environment:
		if ((secondData->type & playerprojectile) == playerprojectile || (secondData->type & enemyprojectile) == enemyprojectile){
			secondData->lifeTime = 1;
		}
		break;
	case enemy:
		if ((secondData->type & playerprojectile) == playerprojectile || (secondData->type & enemyprojectile) == enemyprojectile) {
			if (contact->GetFixtureA()->IsSensor()) {

			}
			else {
				secondData->lifeTime = 1;
				deadly = true;

			}
		}
		break;
	case player:
		if ((secondData->type & playerprojectile) == playerprojectile || (secondData->type & enemyprojectile) == enemyprojectile) {
			secondData->lifeTime = 1;
			deadly = true;
		}

		break;
	}



	if ((firstData->type & enemy )== enemy && (secondData->type & player)== player || ((firstData->type & enemy) == enemy && (secondData->type & playerprojectile) == playerprojectile)) {
		RayCastCallback raycast;
		Transform enemyPoint = entitymanager.getTransformComponent(firstData->id);
		Transform playerPoint = entitymanager.getTransformComponent(secondData->id);
		b2Vec2 epoint = b2Vec2(enemyPoint.position.x, enemyPoint.position.y);
		b2Vec2 target = b2Vec2(playerPoint.position.x, playerPoint.position.y);
		if (epoint != target) {
			contact->GetFixtureA()->GetBody()->GetWorld()->RayCast(&raycast, epoint, target);
		}
		//check the raycast for the amount of objects it has hit between enemy and player/ projectile, if its more than 1 then there is an obstruction.
		if (raycast.environmentHit && raycast.m_hit) {
			obstructed = true;
		}
	}
	b2WorldManifold manifold = b2WorldManifold();
	contact->GetWorldManifold(&manifold);
	glm::vec2 firstNormal = glm::vec2(manifold.normal.x, manifold.normal.y);
	glm::vec2 secondNormal = -firstNormal;
	eventSystem.publishImmediately(new BeginContactEvent(GameplayContext | ImGuiContext | EditorContext, firstData, secondData, deadly, obstructed));
	
}


void geProject::CustomContactListener::EndContact(b2Contact* contact) {
	Entity* firstData = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	Entity* secondData = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	b2WorldManifold manifold = b2WorldManifold();
	contact->GetWorldManifold(&manifold);
	glm::vec2 firstNormal = glm::vec2(manifold.normal.x, manifold.normal.y);
	glm::vec2 secondNormal = -firstNormal;
	eventSystem.publishImmediately(new EndContactEvent(GameplayContext | ImGuiContext | EditorContext, firstData, secondData));}




void geProject::CustomContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
	Entity* firstData = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	Entity* secondData = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	b2WorldManifold manifold = b2WorldManifold();
	contact->GetWorldManifold(&manifold);
	glm::vec2 firstNormal = glm::vec2(manifold.normal.x, manifold.normal.y);
	glm::vec2 secondNormal = -firstNormal;
	eventSystem.publish(new PresolveEvent(GameplayContext | ImGuiContext | EditorContext, firstData, secondData));


}



void geProject::CustomContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
	Entity* firstData = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	Entity* secondData = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	b2WorldManifold manifold = b2WorldManifold();
	contact->GetWorldManifold(&manifold);
	glm::vec2 firstNormal = glm::vec2(manifold.normal.x, manifold.normal.y);
	glm::vec2 secondNormal = -firstNormal;
	eventSystem.publish(new PostsolveEvent(GameplayContext | ImGuiContext | EditorContext, firstData, secondData));

}


