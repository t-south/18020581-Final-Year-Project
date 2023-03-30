#pragma once
#include "Controller.h"
#include "../EntityManager/EntityManager.h"

namespace geProject {
	class PlayerController : public Controller {
	public:
		PlayerController(EntityManager& manager, int entity);
		virtual void dash() override;
		virtual void attack() override;
		virtual void specialAttack() override;
		virtual void shield() override;
		virtual void switchAbility() override;
		virtual void moveTo() override;
	private:
		int entityId;
		EntityManager& eManager;
	};
}