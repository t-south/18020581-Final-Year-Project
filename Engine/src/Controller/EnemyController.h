#pragma once
#include "Controller.h"
#include <queue>
#include "Command.h"

namespace geProject {
	class EnemyController : public Controller {
	public:
		int entityId;
		glm::vec3 desiredVelocity;
		glm::vec3 currentVelocity;
		glm::vec3 steeringVelocity;
		float maxForce{ 0.25f };
		float maxVelocity{ 2 };
		EnemyController(int entity);
		virtual void dash() override;
		virtual void attack() override;
		virtual void specialAttack() override;
		virtual void shield() override;
		virtual void switchAbility() override;
		virtual void moveTo(float x, float y, float dt) override;		
		void rotateTo(float desireDirection, float currentDirection);
		void update(float deltaTime);
	};
}