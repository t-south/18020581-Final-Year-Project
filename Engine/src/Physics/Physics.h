#pragma once
#include "CustomContactListener.h"
#include "RayCastListener.h"
#include <box2d/box2d.h>
#include <unordered_map>
#include <ge_engine/Core.h>
//#include "../EntityManager/EntityManager.h"
#include <ge_engine/Managers.h>

namespace geProject {
	//credit to gamedev.stackexchange.com/questions/196951/how-do-i-correctly-use-userdata-in-box2d
	struct FixtureUserData	{
		int mObjectType{};
		b2Fixture* mOwningFixture{};
	};

	class Physics {
	public:
		Physics(/*EntityManager& eManager*/);
		~Physics();
		void startUp();
	
		void addEntity(int entityId);		
		void addBoxCollider(BoxCollider box, int entityId);
		void addCircleCollider(CircleCollider circle, int entityId);
		void removeEntity(int  entityId);
		void clear();
		void update(float deltaTime); 
		b2Body& getPhysicsBody(int entityId);
		void applyLinearImpulse(int entityId, float x, float y);
		void applyRotation(int entityId, float angle);
		//b2RayCastOutput getRayCast(int entityId, float coordAx, float coordAy, float coordBx, float coordBy);
	private:
		b2Vec2 gravity{0, 0};
		b2World world{b2World(gravity)};
		float time{ 0 };
		float timeStep;
		float velocity;
		float position;
		std::unordered_map<int, b2Body*> bodies;		
		std::vector<FixtureUserData> fixtureData;
		CustomContactListener customCallback;		
		void updateRigidBody(RigidEvent* e);
		void updateBoxCollider(BoxColliderEvent* e);
		void updateCircleCollider(CircleColliderEvent* e);	
		void deleteEntityPhysics(DeleteEntityEvent* e);	
		//EntityManager* entitymanager;
		RayCastListener* rayCast(int entityId, const b2Vec2& origin, const b2Vec2& target);
	};
}