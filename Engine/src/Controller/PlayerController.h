#pragma once
#include "Controller.h"
#include "../EntityManager/EntityManager.h"
#include "../Cameras/Camera.h"
#include <cmath>
#include "../Physics/Physics.h"

namespace geProject {
	class PlayerController : public Controller {
	public:
		PlayerController(EntityManager& manager, Camera& camera, Physics& pManager, int entity);
		virtual void dash() override;
		virtual void attack() override;
		virtual void specialAttack() override;
		virtual void shield() override;
		virtual void switchAbility() override;
		virtual void moveTo(float x, float y) override;
		void rotateToCursor(MouseMoveEvent* mouseMove);
		void update(float deltaTime);
	private:
		float dt;
		int entityId;
		EntityManager& eManager;	
		Camera& playerCamera;
		Physics& physicsManager;
	};
}