#pragma once
#include <box2d/box2d.h>
#include "../EntityManager/EntityManager.h"

namespace geProject {
	class Physics {
	public:
		Physics(EntityManager& emanager);
		~Physics();
		void addEntity(Entity& entity);
		void update(float deltaTime);
	private:
		b2Vec2 gravity;
		b2World world;
		float time;
		float timeStep;
		int velocity;
		int position;
		EntityManager* manager{ nullptr };
	};
}