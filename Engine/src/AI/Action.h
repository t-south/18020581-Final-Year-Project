#pragma once
#include <unordered_map>
#include <string>
#include <ge_engine/Components.h>
#include <ge_engine/Core.h>

namespace geProject {
	
	class Action {
	public:
		float cost{1.0f};
		float actionTime;
		bool inRange{ false };
		bool completed{ false };
		Transform target{.id = 0};			
		float getCost() {return cost;};
		bool isComplete() { return completed; };
		bool isValid() {}; 
		virtual void executeAction(float posx, float posy) = 0;
		virtual int setEffect(int state) = 0;
		virtual bool checkPrecondition(int state) =0;
		virtual int applyPrecondition(int state) = 0;
		
	};

	class DodgeAction : public Action {
	public:
		DodgeAction() {	cost = 5;};		
		
		void executeAction(float posx, float posy) {};

		int setEffect(int state) {
			return state;
		};
		bool checkPrecondition(int state) { int newState = state; newState |= ATTACK_SIGHTED; return newState == state; };
		int applyPrecondition(int state) { state |= ATTACK_SIGHTED; return state; };
	};

	class ShieldAction : public Action {
	public:
		ShieldAction() {cost = 4;};
	
		void executeAction(float posx, float posy) {};
		int setEffect(int state) {
			state = state & ~HAS_ENERGY;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= ATTACK_SIGHTED | HAS_ENERGY; return newState == state; };
		int applyPrecondition(int state) { state |= ATTACK_SIGHTED | HAS_ENERGY; return state; };
	};

	class DashAction : public Action {
	public:
		DashAction() {cost = 4;	};
		
		void executeAction(float posx, float posy) {};

		int setEffect(int state) {		
			state = state & ~HAS_ENERGY;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= ATTACK_SIGHTED | HAS_ENERGY; return newState == state; };
		int applyPrecondition(int state) { state |= ATTACK_SIGHTED | HAS_ENERGY; return state; };
	};

	class RecoverEnergyAction : public Action {
	public:
		RecoverEnergyAction() {};
	
		void executeAction(float posx, float posy) {};

		int setEffect(int state) {
			state = state | HAS_ENERGY;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState &= ~HAS_ENERGY; return newState == state; };
		int applyPrecondition(int state) { state &= ~HAS_ENERGY; return state; };
	};

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

	class InvestigateAction : public Action {
	public:
		InvestigateAction() {};
	
		void executeAction(float posx, float posy) {};

		int setEffect(int state) {
			state = state & ~ALERT;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= ALERT; newState &= ~AT_HOME; return newState == state; };
		int applyPrecondition(int state) { state |= ALERT; state &= ~AT_HOME; return state; };
	};

	class PatrolAction : public Action {
	public:
		PatrolAction() {};
		
		void executeAction(float posx, float posy) {
			if (inRange) {				
				completed = true;
				inRange = false;
				target.id = 0;
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
	};

	class WaitAction : public Action {
	public:
		WaitAction() {};
		
		void executeAction(float posx, float posy) {};

		int setEffect(int state) {
			state = state & ~AGENT_ANGRY;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= AGENT_ANGRY; return newState == state; };
		int applyPrecondition(int state) { state |= AGENT_ANGRY; return state; };

	};

	class GoHomeAction : public Action {
	public:
		GoHomeAction(float posX, float posY) {
			target = Transform{ .position = glm::vec2(posX, posY) };
		};

		void executeAction(float posx, float posy) {
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
	};

	class FireAttackAction : public Action {
	public:
		FireAttackAction() {};

		void executeAction(float posx, float posy) {};

		int setEffect(int state) {
			state = state | ENEMY_DEAD;
			state = state & ~FIRE_ENERGY;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= ENEMY_VISIBLE | FIRE_ENERGY; newState &= ~ENEMY_DEAD; return newState == state; };
		int applyPrecondition(int state) { state |= ENEMY_VISIBLE | FIRE_ENERGY; state &= ~ENEMY_DEAD; return state; };
	};

	class WaterAttackAction : public Action {
	public:
		WaterAttackAction() {};

		void executeAction(float posx, float posy) {};

		int setEffect(int state) {
			state = state | ENEMY_DEAD;
			state = state & ~WATER_ENERGY;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= ENEMY_VISIBLE | WATER_ENERGY; newState &= ~ENEMY_DEAD; return newState == state; };
		int applyPrecondition(int state) { state |= ENEMY_VISIBLE | WATER_ENERGY; state &= ~ENEMY_DEAD; return state; };
	};

	class WindAttackAction : public Action {
	public:
		WindAttackAction() {};

		void executeAction(float posx, float posy) {};
		int setEffect(int state) {
			state = state | ENEMY_DEAD;
			state = state & ~WIND_ENERGY;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= ENEMY_VISIBLE | WIND_ENERGY; newState &= ~ENEMY_DEAD; return newState == state; };
		int applyPrecondition(int state) { state |= ENEMY_VISIBLE | WIND_ENERGY; state &= ~ENEMY_DEAD; return state; };
	};

	class EarthAttackAction : public Action {
	public:
		EarthAttackAction() {};

		void executeAction(float posx, float posy) {};

		int setEffect(int state) {
			state = state | ENEMY_DEAD;
			state = state & ~EARTH_ENERGY;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= ENEMY_VISIBLE | EARTH_ENERGY; newState &= ~ENEMY_DEAD; return newState == state; };
		int applyPrecondition(int state) { state |= ENEMY_VISIBLE | EARTH_ENERGY; state &= ~ENEMY_DEAD; return state; };
	};




}