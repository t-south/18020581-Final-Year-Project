#pragma once
#include "Controller.h"
#include <queue>
#include "Command.h"

namespace geProject {
	class EnemyController : public Controller {
	public:
		int entityId;
		EnemyController(int entity);
		virtual void dash() override;
		virtual void attack() override;
		virtual void specialAttack() override;
		virtual void shield() override;
		virtual void switchAbility() override;
		virtual void moveTo(float x, float y) override;		
		void rotateTo(float rotate);
		void update(float deltaTime);
	};
}