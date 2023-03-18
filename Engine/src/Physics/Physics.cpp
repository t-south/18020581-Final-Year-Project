#include "Physics.h"


geProject::Physics::Physics(EntityManager& emanager):time(0), world(b2World(gravity)) {
	timeStep = 1.0f / 60.0f;
	position = 3.0f;
	velocity = 8.0f;	
	manager = &emanager;
}

geProject::Physics::~Physics(){}


void geProject::Physics::addEntity(Entity& entity) {
	//if(entity.compMask ) // select components based on the mask in the entity
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
	if(circle->id > 0){
		shape.m_radius = circle->radius;
	}
	else if(box->id > 0){
		shape.SetAsBox(box->centre.x, box->centre.y, b2Vec2(box->origin[0], box->origin[1]), 0);
		body.position.Set(body.position.x + box->offset[0], body.position.y + box->offset[1]);		

	}
	b2Body* worldBody = world.CreateBody(&body);
	worldBody->CreateFixture(&shape, rigid->density);
	
}

void geProject::Physics::update(float deltaTime){
	time += deltaTime;
	if (time >= 0.0f) {
		time -= timeStep;
		world.Step(timeStep, velocity, position);
	}

}