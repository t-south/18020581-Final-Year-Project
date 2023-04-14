#pragma once
#include <unordered_map>
#include <string>
#include <ge_engine/Components.h>
#include <ge_engine/Core.h>
#include <ge_engine/Managers.h>
#include <ge_engine/PhysicsManager.h>
#include "../Controller/Command.h"
#include "../Controller/EnemyController.h"

namespace geProject {
	enum actionType {
		COMBAT, LIFESTYLE, DUTY
	};

	class Action {
	public:
		float cost{1.0f};
		float actionTime;
		actionType actiontype;
		bool inRange{ false };
		bool completed{ false };
		Transform target{.id = 0};	
		bool attack{ false };
		int attackType;	
		float getCost() {return cost;};
		bool isComplete() { return completed; };
		bool isValid() {}; 
		virtual void executeAction(float posx, float posy) = 0;
		virtual int setEffect(int state) = 0;
		virtual bool checkPrecondition(int state) =0;
		virtual int applyPrecondition(int state) = 0;
		virtual bool proceduralPrecondition(int agentId) = 0;
		
	};

	class DodgeAction : public Action {
	public:
		DodgeAction() {actiontype = COMBAT; };		
		void executeAction(float posx, float posy) { std::cout << "Dodge Action" << std::endl; };
		int setEffect(int state) {
			return state;
		};
		bool checkPrecondition(int state) { int newState = state; newState |= ATTACK_SIGHTED; return newState == state; };
		int applyPrecondition(int state) { state |= ATTACK_SIGHTED; return state; };
		bool proceduralPrecondition(int agentId) { return true; };
	};

	class ShieldAction : public Action {
	public:
		ShieldAction() {actiontype = COMBAT; };
	
		void executeAction(float posx, float posy) { std::cout << "Shield Action" << std::endl; };
		int setEffect(int state) {
			state = state & ~HAS_ENERGY;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= ATTACK_SIGHTED | HAS_ENERGY; return newState == state; };
		int applyPrecondition(int state) { state |= ATTACK_SIGHTED | HAS_ENERGY; return state; };
		bool proceduralPrecondition(int agentId) { return true; };
	};

	class DashAction : public Action {
	public:
		DashAction() {actiontype = COMBAT;};
		
		void executeAction(float posx, float posy) { std::cout << "Dash Action" << std::endl; };

		int setEffect(int state) {		
			state = state & ~HAS_ENERGY;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= ATTACK_SIGHTED | HAS_ENERGY; return newState == state; };
		int applyPrecondition(int state) { state |= ATTACK_SIGHTED | HAS_ENERGY; return state; };
		bool proceduralPrecondition(int agentId) { return true; };
	};

	class RecoverEnergyAction : public Action {
	public:
		RecoverEnergyAction() {actiontype = COMBAT; };
	
		void executeAction(float posx, float posy) { cost = -1; completed = true; 
			std::cout << "Recover Action" << std::endl;
		};

		int setEffect(int state) {
			state = state | HAS_ENERGY;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState &= ~HAS_ENERGY; return newState == state; };
		int applyPrecondition(int state) { state &= ~HAS_ENERGY; return state; };
		bool proceduralPrecondition(int agentId) { return true; };
	};
	/*
	class PickupObjectAction : public Action {
	public:
		PickupObjectAction() {};
	
		void executeAction(float posx, float posy) {};

		int setEffect(int state) {
			state = state & ~POWERUP_IN_RANGE;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= POWERUP_IN_RANGE; return newState == state; };
		int applyPrecondition(int state) { state |= POWERUP_IN_RANGE; return state; };
	};
	*/
	class InvestigateAction : public Action {
	public:
		InvestigateAction() {actiontype = COMBAT; };
	
		void executeAction(float posx, float posy) { 
			if (inRange) {
				completed = true;
				inRange = false;
				target.id = 0;				
			}			
			std::cout << "Investigate Action" << std::endl; };

		int setEffect(int state) {
			state = state & ~ALERT;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= ALERT; newState &= ~AT_HOME; return newState == state; };
		int applyPrecondition(int state) { state |= ALERT; state &= ~AT_HOME; return state; };
		bool proceduralPrecondition(int agentId) { 
			
			return true; 
		};
	};

	class PatrolAction : public Action {
	public:
		PatrolAction() {actiontype = DUTY; };
		
		void executeAction(float posx, float posy) {
			std::cout << "Patrol Action" << std::endl;
			if (inRange) {				
				completed = true;
				inRange = false;
				target.id = 0;
				std::cout << "patrolled" << std::endl;
			}
			else if (target.id == 0) {
				target = worldstate.getRandomPoint(posx, posy);
			}
		};

		int setEffect(int state) {
			state = state | PATROLLED;
			state = state & ~AT_HOME;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= AT_HOME; newState &= ~ENEMY_VISIBLE & ~PATROLLED; return newState == state; };
		int applyPrecondition(int state) { state |= AT_HOME; state &= ~ENEMY_VISIBLE & ~PATROLLED; return state; };
		bool proceduralPrecondition(int agentId) { return true; };
	};

	class WaitAction : public Action {
	public:
		WaitAction() {actiontype = LIFESTYLE; };
		void executeAction(float posx, float posy) { std::cout << "Wait Action" << std::endl; };

		int setEffect(int state) {
			state = state & ~AGENT_ANGRY;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= AGENT_ANGRY; return newState == state; };
		int applyPrecondition(int state) { state |= AGENT_ANGRY; return state; };
		bool proceduralPrecondition(int agentId) { return true; };
	};

	class GoHomeAction : public Action {
	public:
		GoHomeAction(float posX, float posY) {actiontype = LIFESTYLE;target = Transform{ .position = glm::vec2(posX, posY) };
		};

		void executeAction(float posx, float posy) {
			std::cout << "GoHome Action" << std::endl;
			if (inRange) {
				completed = true;
				inRange = false;				
			}
		};
		int setEffect(int state) {
			state = state | AT_HOME;	
			state = state & ~PATROLLED;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= PATROLLED; newState &= ~ENEMY_VISIBLE & ~AT_HOME; return newState == state; };
		int applyPrecondition(int state) { state |= PATROLLED; state &= ~ENEMY_VISIBLE & ~AT_HOME; return state; };
		bool proceduralPrecondition(int agentId) { return true; };
	};

	class FireAttackAction : public Action {
	public:
		FireAttackAction() { actiontype = COMBAT; };

		void executeAction(float posx, float posy) { 			
			attack = true;
			attackType = dmgTypes::FIRE;
			completed = true;
			
			
			std::cout << "Fire Attack Action" << std::endl; };

		int setEffect(int state) {
			state = state | ENEMY_DEAD;			
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= ENEMY_VISIBLE; newState &= ~ENEMY_DEAD; return newState == state; };
		int applyPrecondition(int state) { 
			
			state |= ENEMY_VISIBLE; 
			state &= ~ENEMY_DEAD; return state; 
		
		};
		bool proceduralPrecondition(int agentId) {
			bool targetFound = true;
			inRange = true;
			Transform playerpos = entitymanager.getTransformComponent(entitymanager.getPlayerId());
			Transform agentpos = entitymanager.getTransformComponent(agentId);		
			if (physicsmanager.checkTargetObstructed(agentpos.position.x, agentpos.position.y, playerpos.position.x, playerpos.position.y)) {
				target = playerpos;
				targetFound = false;
				inRange = true;
			}			
			return targetFound;
		};
	};

	class WaterAttackAction : public Action {
	public:
		WaterAttackAction() { actiontype = COMBAT; };

		void executeAction(float posx, float posy) { 
			
			attack = true;
			attackType = dmgTypes::WATER;
			completed = true;
			std::cout << "Water Attack Action" << std::endl; };

		int setEffect(int state) {
			state = state | ENEMY_DEAD;			
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= ENEMY_VISIBLE; newState &= ~ENEMY_DEAD; return newState == state; };
		int applyPrecondition(int state) { state |= ENEMY_VISIBLE; state &= ~ENEMY_DEAD; return state; };
		bool proceduralPrecondition(int agentId) { 
			bool targetFound = true;
			inRange = true;
			Transform playerpos = entitymanager.getTransformComponent(entitymanager.getPlayerId());
			Transform agentpos = entitymanager.getTransformComponent(agentId);
			if (physicsmanager.checkTargetObstructed(agentpos.position.x, agentpos.position.y, playerpos.position.x, playerpos.position.y)) {
				target = playerpos;
				targetFound = false;
				inRange = false;
			}
			return targetFound;
		};
	};

	class WindAttackAction : public Action {
	public:
		WindAttackAction() { actiontype = COMBAT; };

		void executeAction(float posx, float posy) { 
			
			attack = true;
			attackType = dmgTypes::LIGHTNING;
			
			completed = true;
			std::cout << "Wind Attack Action" << std::endl; };
		int setEffect(int state) {
			state = state | ENEMY_DEAD;			
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= ENEMY_VISIBLE; newState &= ~ENEMY_DEAD; return newState == state; };
		int applyPrecondition(int state) { state |= ENEMY_VISIBLE; state &= ~ENEMY_DEAD; return state; };
		bool proceduralPrecondition(int agentId) {
			bool targetFound = true;
			inRange = true;
			Transform playerpos = entitymanager.getTransformComponent(entitymanager.getPlayerId());
			Transform agentpos = entitymanager.getTransformComponent(agentId);
			if (physicsmanager.checkTargetObstructed(agentpos.position.x, agentpos.position.y, playerpos.position.x, playerpos.position.y)) {
				target = playerpos;
				targetFound = false;
				inRange = false;
			}
			return targetFound;
		};
	};

	class EarthAttackAction : public Action {
	public:
		EarthAttackAction() { actiontype = COMBAT; };

		void executeAction(float posx, float posy) { 
		
			attack = true;
			attackType = dmgTypes::EARTH;
			completed = true;
			std::cout << "Earth Attack Action" << std::endl; };

		int setEffect(int state) {
			state = state | ENEMY_DEAD;			
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= ENEMY_VISIBLE ; newState &= ~ENEMY_DEAD; return newState == state; };
		int applyPrecondition(int state) { state |= ENEMY_VISIBLE ; state &= ~ENEMY_DEAD; return state; };
		bool proceduralPrecondition(int agentId) { 
			bool targetFound = true;
			inRange = true;
			Transform playerpos = entitymanager.getTransformComponent(entitymanager.getPlayerId());
			Transform agentpos = entitymanager.getTransformComponent(agentId);		
			if (physicsmanager.checkTargetObstructed(agentpos.position.x, agentpos.position.y, playerpos.position.x, playerpos.position.y)) {
				target = playerpos;
				targetFound = false;
				inRange = false;
			}			
			return targetFound;
		};
	};




}