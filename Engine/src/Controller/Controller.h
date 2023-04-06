#pragma once
#include <ge_engine/Core.h>


namespace geProject {
	class Controller {	
	public:
		virtual ~Controller() {};
		virtual void dash() = 0;
		virtual void attack() = 0;
		virtual void specialAttack() = 0;
		virtual void shield() = 0;
		virtual void switchAbility() = 0;
		virtual void moveTo(float x, float y) =0;
		virtual void rotateTo(float rotate) = 0;
	};
}