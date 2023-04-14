#include "Physics.h"


geProject::Physics::Physics() {}

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
		if ((entity.compMask & 0x0200) == 0x0200) {			//View collider
			ViewCollider view = entitymanager.getViewComponent(entity.id);
			addViewSensorCollider(view, entity.id);
			
		}
	}	
}

bool geProject::Physics::checkTargetObstructed(float startx, float starty, float targetx, float targety){
	bool obstructed = false;
	RayCastCallback raycast;
	world.RayCast(&raycast, b2Vec2(startx, starty), b2Vec2(targetx, targety));	
	//check the raycast for the amount of objects it has hit between enemy and player/ projectile, if its more than 1 then there is an obstruction.
 	if (raycast.environmentHit && raycast.m_hit) {
		obstructed = true;
	}
	/*
	for (auto& hit : raycast.raycasthit) {
		std::cout << hit.type << " -> ";
	}
	std::cout << std::endl;
	std::cout << raycast.m_hit << std::endl;
	*/
	std::cout << obstructed << std::endl;
	return obstructed;
}


void geProject::Physics::addBoxCollider(BoxCollider box, int entityId) {
	b2PolygonShape shape = b2PolygonShape();	
	b2Body& body = *bodies[entityId];
	b2FixtureDef shapeFixture;
	shape.SetAsBox(box.boxSize[0] * 0.5f, box.boxSize[1] * 0.5f, b2Vec2(box.offset[0], box.offset[1]), 0);
	shapeFixture.shape = &shape;	
	shapeFixture.density = 1;
	shapeFixture.filter.categoryBits = box.entityType;
	shapeFixture.filter.maskBits = box.colliders;
	shapeFixture.isSensor = box.sensor;
	body.CreateFixture(&shapeFixture);
	//std::cout << "added physics objects: " << bodies.size() << std::endl;	
}

void geProject::Physics::addCircleCollider(CircleCollider circle, int entityId) {
	b2CircleShape shape = b2CircleShape();	
	b2Body& body = *bodies[entityId];
	b2FixtureDef shapeFixture;
	shape.m_p.Set(circle.offset[0], circle.offset[1]);
	shape.m_radius = circle.radius;
	shapeFixture.shape = &shape;
	shapeFixture.density = 1;
	shapeFixture.filter.categoryBits = circle.entityType;
	shapeFixture.filter.maskBits = circle.colliders;
	shapeFixture.isSensor = circle.sensor;
	body.CreateFixture(&shapeFixture);
}

void geProject::Physics::addViewSensorCollider(ViewCollider view, int entityId){
	b2PolygonShape shape = b2PolygonShape();
	b2Body& body = *bodies[entityId];
	b2FixtureDef shapeFixture;
	float radius = view.radius;
	b2Vec2 vertices[8]{};
	vertices[0].Set(0, 0);
	double pi = 3.14159265;
	for (int i = 0; i < 7; i++) {
		float angle = i / 6.0 * 90 * (pi/180);
		vertices[i + 1].Set(radius * cosf(angle + 0.7855f), radius * sinf(angle + 0.7855f));
	}
	shape.Set(vertices, 8);
	shapeFixture.shape = &shape;
	//shapeFixture.density = 1;
	shapeFixture.isSensor = view.sensor;	

	shapeFixture.filter.categoryBits = view.entityType;
	shapeFixture.filter.maskBits = view.colliders;
	//shapeFixture.isSensor = view.sensor;
	body.CreateFixture(&shapeFixture);
}




void geProject::Physics::removeEntity(int entityId){
	if (bodies.find(entityId) != bodies.end()) {
		b2Fixture* fixtures = bodies[entityId]->GetFixtureList();
		while (fixtures != nullptr) {			
			Entity* userData = (Entity*)fixtures->GetUserData().pointer;
			if (userData != nullptr) {
				delete userData;
			}
			auto tmp = fixtures->GetNext();
			fixtures->GetBody()->DestroyFixture(fixtures);
			fixtures = tmp;
		}
		bodies[entityId]->GetWorld()->DestroyBody(bodies[entityId]);
		
		bodies.erase(entityId);
		//std::cout << "removed physics objects: " << bodies.size() << std::endl;
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
	dt = deltaTime;
	if (time >= 0.0f) {
		time -= timeStep;
		world.Step(timeStep, velocity, position);
		if (bodies.size() > 0) {			
			for (auto &body : bodies) {
				if (body.second->GetType() == b2_dynamicBody) {
					
				}
				float test = body.second->GetAngle();
				b2Vec2 position = body.second->GetPosition();
				double pi = 3.14159265;

				float angle = (float)(body.second->GetAngle() * (180.0/ pi));
				
				Entity* bodyData = (Entity*)body.second->GetUserData().pointer;
				/*
				if (body.first == 1428) {	
					for (b2Fixture* fixture = body.second->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
						std::cout << body.first << ": " << fixture->GetBody()->GetTransform().p.x << "  " << fixture->GetBody()->GetTransform().p.y << std::endl;
					}
					//std::cout << "lVel: " << body.second->GetLinearVelocity().x << " " << body.second->GetLinearVelocity().y << " angle: " << angle << std::endl;
					//std::cout << " posx: " << position.x << " posy: " << position.y << " inertia: " << body.second->GetInertia() << std::endl;
					
				}*/

		
				entitymanager.updateTransform(body.first, position.x, position.y, angle);
				if (bodyData != nullptr && (bodyData->type == enemyprojectile || bodyData->type == playerprojectile) && bodyData->lifeTime > -1) {
					if (bodyData->lifeTime == 0) {										
						deleteBodies.push_back(bodyData->id);
					}
					else {
						bodyData->lifeTime -= 1;
					}
				}
				//printf("%4.2f %4.2f %4.2f %u %4.2f\n", position.x, position.y, angle, (int)body.first, body.second->GetMass());	
				
				/*for (b2Fixture* fixture = body.second->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
					std::cout << count << ": " << fixture->GetBody()->GetTransform().p.x << "  " << fixture->GetBody()->GetTransform().p.y << std::endl;
				}*/
				

				
				//eventSystem.publishImmediately(new TransformEvent(ImGuiContext | GameplayContext, (int)body.first, position.x, position.y, angle));
			}

			for (auto& body : deleteBodies) {
				//calls delete entity -> delete render -> delete physics component
				entitymanager.deleteEntity(body);
			}
			deleteBodies.clear();
			//publish events from earlier callbacks
			//eventSystem.handleEvents(beginContact);
			//eventSystem.handleEvents(endContact);
		}		
	}


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

void geProject::Physics::applyAngularVelocity(int entityId, float angle){
	bodies[entityId]->SetAngularVelocity(angle);
}

void geProject::Physics::createProjectile(int entityId){
	Damage dmg = entitymanager.getDamageComponent(entityId);
	Entity parent = entitymanager.getEntity(entityId);
	int projectileId = 0;
	if (parent.type == enemy) {
		projectileId = entitymanager.addEntity(enemyprojectile);
	}
	else if (parent.type == player) {
		projectileId = entitymanager.addEntity(enemyprojectile);
	}
	
	entitymanager.addParentEntity(entityId, projectileId);
	Entity entity = entitymanager.getEntity(projectileId);
	int colliders =0;
	if (entity.type == enemy) {
		colliders |= BOUNDARY | PLAYER;
		entity.type = enemyprojectile;
	}
	else if (entity.type == player) {
		colliders |= BOUNDARY | ENEMY;
		entity.type = playerprojectile;
	}
	Transform transform = entitymanager.getTransformComponent(entityId);
	b2BodyDef body;
	double pi = 3.14159265;
	float angle = bodies[entityId]->GetAngle();
	body.angle = angle;
	body.position.Set(transform.position.x, transform.position.y);
	body.angularDamping = 0;
	body.linearDamping = 0;
	body.fixedRotation = 0;
	body.bullet = false;	
	b2Body* worldBody{};
	b2Vec2 velocity;
	switch (dmg.dmgType) {
	case dmgTypes::FIRE:
		entity.lifeTime = 100;
		body.type = b2_dynamicBody;
		entitymanager.assignSpriteRender(projectileId, SpriteRender{ .color = glm::vec4(1, 0 ,0, 1) });
		entitymanager.assignCircleCollider(projectileId, CircleCollider{ .radius = 0.1f, .entityType = PROJECTILE,  .colliders = colliders });
		velocity = b2Vec2(cos(angle + 1.5708) * 2.0f, sin(angle + 1.5708) * 2.0f);
		body.userData.pointer = reinterpret_cast<uintptr_t>(new Entity(entity));
		worldBody = world.CreateBody(&body);
		worldBody->SetLinearVelocity(velocity);
		break;
	case dmgTypes::LIGHTNING:
		entity.lifeTime = 100;
		body.type = b2_dynamicBody;
		entitymanager.assignSpriteRender(projectileId, SpriteRender{ .color = glm::vec4(0.4226621091365814f, 0.127809539437294f ,0.9026548862457275f, 1) });
		entitymanager.assignCircleCollider(projectileId, CircleCollider{ .radius = 0.1f, .entityType = PROJECTILE,  .colliders = colliders });
		velocity = b2Vec2(cos(angle + 1.5708) * 2.0f, sin(angle + 1.5708) * 2.0f);
		body.userData.pointer = reinterpret_cast<uintptr_t>(new Entity(entity));
		worldBody = world.CreateBody(&body);
		worldBody->SetLinearVelocity(velocity);
		break;
	case dmgTypes::EARTH:
		entity.lifeTime = 50;
		body.type = b2_dynamicBody;
		entitymanager.assignSpriteRender(projectileId, SpriteRender{ .color = glm::vec4(1, 1 ,1, 1) });
		entitymanager.assignCircleCollider(projectileId, CircleCollider{ .radius = 0.1f, .entityType = PROJECTILE,  .colliders = colliders });
		velocity = b2Vec2(cos(angle + 1.5708) * 2.0f, sin(angle + 1.5708) * 2.0f);
		body.userData.pointer = reinterpret_cast<uintptr_t>(new Entity(entity));
		worldBody = world.CreateBody(&body);
		worldBody->SetLinearVelocity(velocity);
		body.type = b2_staticBody;
		break;
	case dmgTypes::WATER:
		entity.lifeTime = 50;
		body.type = b2_dynamicBody;
		entitymanager.assignSpriteRender(projectileId, SpriteRender{ .color = glm::vec4(0, 0 ,1, 1) });
		entitymanager.assignCircleCollider(projectileId, CircleCollider{ .radius = 0.1f, .entityType = PROJECTILE,  .colliders = colliders });
		velocity = b2Vec2(cos(angle + 1.5708) * 2.0f, sin(angle + 1.5708) * 2.0f);
		body.userData.pointer = reinterpret_cast<uintptr_t>(new Entity(entity));
		worldBody = world.CreateBody(&body);
		worldBody->SetLinearVelocity(velocity);
		break;
	default:
		break;
	}

	bodies[entity.id] = worldBody;
	b2CircleShape shape = b2CircleShape();
	b2FixtureDef shapeFixture;
	entitymanager.assignTransform(projectileId, Transform{ .position = glm::vec2(transform.position.x, transform.position.y), .scale = { glm::vec2(0.1f, 0.1f)}, .rotation = angle });
	for (auto& circle : entitymanager.getCircleColliderComponents(projectileId)) {
		shape.m_p.Set(circle.offset[0], circle.offset[1]);
		shape.m_radius = circle.radius;
		shapeFixture.shape = &shape;
		shapeFixture.density = 1;
		shapeFixture.filter.categoryBits = circle.entityType;
		shapeFixture.filter.maskBits = circle.colliders;
		shapeFixture.isSensor = circle.sensor;
		worldBody->CreateFixture(&shapeFixture);
	}
		

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

/*

geProject::RayCastListener* geProject::Physics::rayCast(int entityId, const b2Vec2& origin, const b2Vec2& target){
	RayCastListener* raycastCallback = new RayCastListener(entityId);
	world.RayCast(raycastCallback, origin, target);
	return raycastCallback;
}
*/


