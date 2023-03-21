#include "Physics.h"


geProject::Physics::Physics(EntityManager& emanager):time(0), world(b2World(gravity)) {
	timeStep = 1.0f / 60.0f;
	position = 3.0f;
	velocity = 8.0f;	
	time = 0.0f;
	eventSystem.subscribe(this, &Physics::updateRigidBody);
	eventSystem.subscribe(this, &Physics::updateCircleCollider);
	eventSystem.subscribe(this, &Physics::updateBoxCollider);
	manager = &emanager;
}

geProject::Physics::~Physics(){}


void geProject::Physics::addEntity(Entity& entity) {
	//if(entity.compMask ) // select components based on the mask in the entity
	if (bodies.count(entity.id) == 0 && (entity.compMask & 4) == 4) {
		//if there is a rigidbody make a new body		
		auto rigid = manager->getRigidBodyComponent(entity.id);
		auto transform = manager->getTransformComponent(entity.id);
		b2BodyDef body;
		body.angle = transform->rotation;
		body.position.Set(transform->position.x, transform->position.y);
		body.angularDamping = rigid->angularDamping;
		body.linearDamping = rigid->linearDamping;
		body.fixedRotation = rigid->fixedRotate;
		body.bullet = rigid->bullet;
		switch (rigid->bodyType) {
		case 0:
			body.type = b2_kinematicBody;
			break;
		case 1:
			body.type = b2_dynamicBody;
			break;
		case 2:
			body.type = b2_staticBody;
			break;
		default:
			break;
		}
		b2PolygonShape shape = b2PolygonShape();
		auto circle = manager->getCircleColliderComponent(entity.id);
		//should i use mask or check id of components returned from entity manager?
		auto box = manager->getBoxColliderComponent(entity.id);
		if (circle->id > 0) {
			shape.m_radius = circle->radius;
		}
		else if (box->id > 0) {
			shape.SetAsBox(transform->centre.x, transform->centre.y, b2Vec2(box->origin[0], box->origin[1]), 0);
			body.position.Set(body.position.x + box->offset[0], body.position.y + box->offset[1]);

		}
		b2Body* worldBody = world.CreateBody(&body);
		bodies[entity.id] = worldBody;
		worldBody->CreateFixture(&shape, rigid->density);
	}

	
}

void geProject::Physics::removeEntity(Entity& entity){
	if (bodies[entity.id] != NULL) {
		bodies.erase(entity.id);
	}
}

void geProject::Physics::clear(){
	bodies.clear();
}

void geProject::Physics::update(float deltaTime){
	time += deltaTime;
	if (time >= 0.0f) {
		time -= timeStep;
		world.Step(timeStep, velocity, position);
		if (bodies.size() > 0) {
			for (auto &body : bodies) {
				b2Vec2 position = body.second->GetPosition();
				float angle = bodies[0]->GetAngle();
				printf("%4.2f %4.2f %4.2f %4.2f\n", position.x, position.y, angle, (int)body.first);
				eventSystem.publishImmediately(new TransformEvent((int)body.first, position.x, position.y, angle));
			}

		}
		
	}

}


void geProject::Physics::updateRigidBody(RigidEvent* event){
	auto body = bodies[event->entityId];	
	if (body != nullptr) {
		auto fixture = body->GetFixtureList();
		fixture->SetDensity(event->rigidbody->density);
		body->ResetMassData();
		bodies[event->entityId] = body;
	}
}

void geProject::Physics::updateBoxCollider(BoxColliderEvent* event){
}

void geProject::Physics::updateCircleCollider(CircleColliderEvent* event){
}
