#pragma once
#include "Controller.h"
#include "../Cameras/Camera.h"
#include "../Physics/Physics.h"
#include "../EntityManager/EntityManager.h"
#include <cmath>



namespace geProject {
	class PlayerController : public Controller {
	public:
		PlayerController(EntityManager& entitymanager, Physics& physicsmanager, Camera& camera, int entity);
		virtual void dash() override;
		virtual void attack() override;
		virtual void specialAttack() override;
		virtual void shield() override;
		virtual void switchAbility() override;
		virtual void moveTo(float x, float y) override;
		void rotateToCursor(MouseMoveEvent* mouseMove);
		void rotateTo(float rotate);
		void update(float deltaTime);
	private:
		float dt;
		int entityId;		
		Camera& playerCamera;
		EntityManager& entitymanager;
		Physics& physicsmanager;
	};
}