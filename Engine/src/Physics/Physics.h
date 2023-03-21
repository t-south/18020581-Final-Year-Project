#pragma once

#include <box2d/box2d.h>
#include <ge_engine/Core.h>
#include <unordered_map>
#include "../EntityManager/EntityManager.h"

namespace geProject {
	class Physics {
	public:
		Physics(EntityManager& emanager);
		~Physics();
		void addEntity(Entity& entity);
		void removeEntity(Entity& entity);
		void clear();
		void update(float deltaTime);
	private:
		b2Vec2 gravity{0, -10.0f};
		b2World world;
		float time;
		float timeStep;
		int velocity;
		int position;
		std::unordered_map<unsigned int, b2Body*> bodies;
		EntityManager* manager{ nullptr };	
		void updateRigidBody(RigidEvent* event);
		void updateBoxCollider(BoxColliderEvent* event);
		void updateCircleCollider(CircleColliderEvent* event);
	};
}