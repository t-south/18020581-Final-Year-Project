#pragma once
#include "CustomContactListener.h"
#include "RayCastListener.h"
#include <box2d/box2d.h>
#include <ge_engine/Core.h>
#include <unordered_map>
#include "../EntityManager/EntityManager.h"

namespace geProject {
	//credit to gamedev.stackexchange.com/questions/196951/how-do-i-correctly-use-userdata-in-box2d
	struct FixtureUserData	{
		int mObjectType{};
		b2Fixture* mOwningFixture{};
	};

	class Physics {
	public:
		Physics(EntityManager& emanager);
		~Physics();
		void addEntity(Entity& entity);
		void addBoxCollider(BoxCollider& box);
		void addCircleCollider(CircleCollider& circle);
		void removeEntity(int  entityId);
		void clear();
		void update(float deltaTime);
		//b2RayCastOutput getRayCast(int entityId, float coordAx, float coordAy, float coordBx, float coordBy);
	private:
		b2Vec2 gravity{0, -10.0f};
		b2World world;
		float time;
		float timeStep;
		int velocity;
		int position;
		std::unordered_map<int, b2Body*> bodies;
		EntityManager* manager{ nullptr };	
		std::vector<FixtureUserData> fixtureData;
		CustomContactListener customCallback;
		
		void updateRigidBody(RigidEvent* e);
		void updateBoxCollider(BoxColliderEvent* e);
		void updateCircleCollider(CircleColliderEvent* e);	
		void deleteEntityPhysics(DeleteEntityEvent* e);	

		RayCastListener* rayCast(int entityId, const b2Vec2& origin, const b2Vec2& target);
	};
}