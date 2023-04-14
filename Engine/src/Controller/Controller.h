#pragma once
#include <ge_engine/Core.h>
#include <ge_engine/PhysicsManager.h>

namespace geProject {
	class Controller {	
	public:
		
		virtual ~Controller() {};
		virtual void dash() = 0;
		virtual void attack() = 0;
		virtual void specialAttack() = 0;
		virtual void shield() = 0;
		virtual void switchAbility() = 0;
		virtual void moveTo(float x, float y, float dt) =0;
		virtual void rotateTo(float desiredDirection, float currentDirection) = 0;
	};
}