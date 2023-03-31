#include "CustomContactListener.h"


geProject::CustomContactListener::CustomContactListener(){
	
}

geProject::CustomContactListener::~CustomContactListener(){

}

void geProject::CustomContactListener::BeginContact(b2Contact* contact) {	
	Entity* firstData = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	Entity* secondData = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	b2WorldManifold manifold = b2WorldManifold();
	contact->GetWorldManifold(&manifold);
	glm::vec2 firstNormal = glm::vec2(manifold.normal.x, manifold.normal.y);
	glm::vec2 secondNormal = -firstNormal;
	eventSystem.publishImmediately(new BeginContactEvent(GameplayContext | ImGuiContext, firstData, secondData, true));
	//put callbacks into handler queue instead of publishing immediately due to multiple collisions occuring per frame 		
}


void geProject::CustomContactListener::EndContact(b2Contact* contact) {
	Entity* firstData = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	Entity* secondData = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	b2WorldManifold manifold = b2WorldManifold();
	contact->GetWorldManifold(&manifold);
	glm::vec2 firstNormal = glm::vec2(manifold.normal.x, manifold.normal.y);
	glm::vec2 secondNormal = -firstNormal;
	eventSystem.publishImmediately(new EndContactEvent(GameplayContext | ImGuiContext, firstData, secondData));}




void geProject::CustomContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
	Entity* firstData = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	Entity* secondData = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	b2WorldManifold manifold = b2WorldManifold();
	contact->GetWorldManifold(&manifold);
	glm::vec2 firstNormal = glm::vec2(manifold.normal.x, manifold.normal.y);
	glm::vec2 secondNormal = -firstNormal;
	eventSystem.publish(new PresolveEvent(GameplayContext | ImGuiContext, firstData, secondData));


}



void geProject::CustomContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
	Entity* firstData = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	Entity* secondData = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	b2WorldManifold manifold = b2WorldManifold();
	contact->GetWorldManifold(&manifold);
	glm::vec2 firstNormal = glm::vec2(manifold.normal.x, manifold.normal.y);
	glm::vec2 secondNormal = -firstNormal;
	eventSystem.publish(new PostsolveEvent(GameplayContext | ImGuiContext, firstData, secondData));

}


