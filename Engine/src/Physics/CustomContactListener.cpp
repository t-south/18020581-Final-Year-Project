#include "CustomContactListener.h"


geProject::CustomContactListener::CustomContactListener(){

}

geProject::CustomContactListener::~CustomContactListener(){

}

void geProject::CustomContactListener::BeginContact(b2Contact* contact) {
	auto firstData = (Entity&)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	auto secondData = (Entity&)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

	b2WorldManifold manifold = b2WorldManifold();
	contact->GetWorldManifold(&manifold);
	glm::vec2 firstNormal = glm::vec2(manifold.normal.x, manifold.normal.y);
	glm::vec2 secondNormal = -firstNormal;

		
}

	
	
	//
	//
	//


void geProject::CustomContactListener::EndContact(b2Contact* contact) {}
	//BoxCollider* firstData = (BoxCollider*)contact->GetFixtureA()->GetUserData();
	//BoxCollider* secondData = (BoxCollider*)contact->GetFixtureB()->GetUserData();
	//b2WorldManifold manifold = b2WorldManifold();
	//contact->GetWorldManifold(manifold);
	//glm::vec2 firstNormal = glm::vec2(manifold.normal.x, manifold.normal.y);
	//glm::vec2 secondNormal = -firstNormal;
	//for (const auto& component : firstData.pointer) {
	//	component
	//}


void geProject::CustomContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {}
	//BoxCollider* firstData = (BoxCollider*)contact->GetFixtureA()->GetUserData();
	//BoxCollider* secondData = (BoxCollider*)contact->GetFixtureB()->GetUserData();
	//b2WorldManifold manifold = b2WorldManifold();
	//contact->GetWorldManifold(manifold);
	//glm::vec2 firstNormal = glm::vec2(manifold.normal.x, manifold.normal.y);
	//glm::vec2 secondNormal = -firstNormal;
	//for (const auto& component : firstData.pointer) {
	//	component
	//}


void geProject::CustomContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {}
	//BoxCollider* firstData = (BoxCollider*)contact->GetFixtureA()->GetUserData();
	//BoxCollider* secondData = (BoxCollider*)contact->GetFixtureB()->GetUserData();
	//b2WorldManifold manifold = b2WorldManifold();
	//contact->GetWorldManifold(manifold);
	//glm::vec2 firstNormal = glm::vec2(manifold.normal.x, manifold.normal.y);
	//glm::vec2 secondNormal = -firstNormal;
	//for (const auto& component : firstData.pointer) {
	//	component
	//}

