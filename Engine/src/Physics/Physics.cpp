#include "Physics.h"


geProject::Physics::Physics(/*/EntityManager& emanager*/) {
	/*
	timeStep = 1.0f / 60.0f;
	position = 3.0f;
	velocity = 0.0f;
	time = 0.0f;
	world.SetContactListener(&customCallback);
	eventSystem.subscribe(this, &Physics::updateRigidBody);
	eventSystem.subscribe(this, &Physics::updateCircleCollider);
	eventSystem.subscribe(this, &Physics::updateBoxCollider);
	eventSystem.subscribe(this, &Physics::deleteEntityPhysics);
	//entitymanager = &emanager;
	*/
}

geProject::Physics::~Physics(){}


void geProject::Physics::startUp(){
	timeStep = 1.0f / 60.0f;
	position = 3.0f;
	velocity = 0.0f;
	time = 0.0f;
	world.SetContactListener(&customCallback);
	eventSystem.subscribe(this, &Physics::updateRigidBody);
	eventSystem.subscribe(this, &Physics::updateCircleCollider);
	eventSystem.subscribe(this, &Physics::updateBoxCollider);
	eventSystem.subscribe(this, &Physics::deleteEntityPhysics);
}


void geProject::Physics::addEntity(int entityId) {
	Entity entity = entitymanager.getEntity(entityId);
	//if(entity.compMask ) // select components based on the mask in the entity
	//std::cout << bodies.count(entity.id) << std::endl;
	if (entity.id > -1 && bodies.count(entity.id) == 0 && (entity.compMask & 4) == 4) {		
		//if there is a rigidbody make a new body	
		Rigidbody rigid = entitymanager.getRigidBodyComponent(entity.id);
		Transform transform = entitymanager.getTransformComponent(entity.id);
		b2BodyDef body;
		double pi = 3.14159265;
		float angle = transform.rotation * pi / 180.0;
		body.angle = angle;
		body.position.Set(transform.position.x, transform.position.y);
		body.angularDamping = rigid.angularDamping;
		body.linearDamping = rigid.linearDamping;
		body.fixedRotation = rigid.fixedRotate;		
		body.bullet = rigid.bullet;		
		body.userData.pointer = reinterpret_cast<uintptr_t>(new Entity(entity));
		switch (rigid.bodyType) {
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
		b2Body* worldBody = world.CreateBody(&body);
		bodies[entity.id] = worldBody;
		
		if ((entity.compMask & 8) == 8) {					//Circle collider
			for (auto& circle: entitymanager.getCircleColliderComponents(entity.id)) {
				addCircleCollider(circle, entity.id);
			}
		}
		if ((entity.compMask & 16) == 16) {			//Box collider
			for (auto& box : entitymanager.getBoxColliderComponents(entity.id)) {
				addBoxCollider(box, entity.id);
			}
		}
	}	
}


void geProject::Physics::addBoxCollider(BoxCollider box, int entityId) {
	b2PolygonShape shape = b2PolygonShape();	
	b2Body& body = *bodies[entityId];
	b2FixtureDef shapeFixture;
	shape.SetAsBox(box.boxSize[0] * 0.5f, box.boxSize[1] * 0.5f, b2Vec2(box.offset[0], box.offset[1]), 0);
	shapeFixture.shape = &shape;	
	shapeFixture.density = 1;
	shapeFixture.isSensor = box.sensor;
	body.CreateFixture(&shapeFixture);
	std::cout << "added physics objects: " << bodies.size() << std::endl;	
}

void geProject::Physics::addCircleCollider(CircleCollider circle, int entityId) {
	b2CircleShape shape = b2CircleShape();	
	b2Body& body = *bodies[entityId];
	b2FixtureDef shapeFixture;
	shape.m_p.Set(circle.offset[0], circle.offset[1]);
	shape.m_radius = circle.radius;
	shapeFixture.shape = &shape;
	shapeFixture.density = 1;
	shapeFixture.isSensor = circle.sensor;
	body.CreateFixture(&shapeFixture);
}




void geProject::Physics::removeEntity(int entityId){
	if (bodies.find(entityId) != bodies.end()) {
		b2Fixture* fixtures = bodies[entityId]->GetFixtureList();
		while (fixtures != nullptr) {
			auto userData = fixtures->GetUserData().pointer;
			if (userData != 0) {
				delete& userData;
			}
			auto tmp = fixtures->GetNext();
			bodies[entityId]->DestroyFixture(tmp);
			fixtures = tmp;
		}
		bodies.erase(entityId);
		std::cout << "removed physics objects: " << bodies.size() << std::endl;
	}
}

void geProject::Physics::clear(){
	for (auto& body : bodies) {
		// Do stuff
		world.DestroyBody(body.second);
	}
	bodies.clear();
}

void geProject::Physics::update(float deltaTime){
	time += deltaTime;
	if (time >= 0.0f) {
		time -= timeStep;
		world.Step(timeStep, velocity, position);
		if (bodies.size() > 0) {
			int count = 0;
			for (auto &body : bodies) {
				if (body.second->GetType() == b2_dynamicBody) {
					
				}
				float test = body.second->GetAngle();
				b2Vec2 position = body.second->GetPosition();
				double pi = 3.14159265;
				
				float angle = (float)(body.second->GetAngle() * (180.0/ pi));
				
				//printf("%4.2f %4.2f %4.2f %u %4.2f\n", position.x, position.y, angle, (int)body.first, body.second->GetMass());	
				
				/*for (b2Fixture* fixture = body.second->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
					std::cout << count << ": " << fixture->GetBody()->GetTransform().p.x << "  " << fixture->GetBody()->GetTransform().p.y << std::endl;
				}*/
				count++;
				entitymanager.updateTransform(body.first, position.x, position.y, (int)angle);
				//eventSystem.publishImmediately(new TransformEvent(ImGuiContext | GameplayContext, (int)body.first, position.x, position.y, angle));
			}
		}		
	}
	//publish events from earlier callbacks
	//eventSystem.handleEvents(beginContact);
	//eventSystem.handleEvents(endContact);
}

b2Body& geProject::Physics::getPhysicsBody(int entityId)
{
	return *bodies[entityId];
}

void geProject::Physics::applyLinearImpulse(int entityId, float x, float y){
	bodies[entityId]->ApplyLinearImpulse(b2Vec2(x, y), bodies[entityId]->GetWorldCenter(), true);
}

void geProject::Physics::applyRotation(int entityId, float angle){
	bodies[entityId]->SetTransform(bodies[entityId]->GetPosition(), angle);
}




//b2RayCastOutput geProject::Physics::getRayCast(int entityId, float coordAx, float coordAy, float coordBx, float coordBy){
	//b2RayCastCallback raycastCb = b2RayCastCallback(entityId);
	//world.RayCast()
	//return 0;
//}


void geProject::Physics::updateRigidBody(RigidEvent* e){
	if (bodies.count(e->entityId) == 1) {
		b2Body* body = bodies[e->entityId];		
		for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()){
			fixture->SetDensity(e->rigidbody->density);		
		}		
		//body->ResetMassData();
		bodies[e->entityId] = body;
	}
}

void geProject::Physics::updateBoxCollider(BoxColliderEvent* e){	
	if (bodies.count(e->entityId) == 1) {
		b2Body* body = bodies[e->entityId];
		b2Fixture* fixture = body->GetFixtureList();
		b2Vec2 position = body->GetPosition();			
		position.x += e->offSetX;
		position.y += e->offsetY;
		b2PolygonShape shape = b2PolygonShape();
		body->DestroyFixture(body->GetFixtureList());
		double angle = body->GetAngle();
		body->SetTransform(position, angle);
		float density = body->GetMass();
		shape.SetAsBox(e->boxSizeX, e->boxSizeY, position, angle);		
		bodies[e->entityId] = body;
		body->CreateFixture(&shape, density);
	}
}



void geProject::Physics::updateCircleCollider(CircleColliderEvent* e){	
	if (bodies.count(e->entityId) == 1) {
		auto circle = bodies[e->entityId];
	}
	
}




void geProject::Physics::deleteEntityPhysics(DeleteEntityEvent* e){	
	removeEntity(e->entityId);
	
}



geProject::RayCastListener* geProject::Physics::rayCast(int entityId, const b2Vec2& origin, const b2Vec2& target){
	RayCastListener* raycastCallback = new RayCastListener(entityId);
	world.RayCast(raycastCallback, origin, target);
	return raycastCallback;
}


