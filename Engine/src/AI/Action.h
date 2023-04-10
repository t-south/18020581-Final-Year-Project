#pragma once
#include <unordered_map>
#include <string>
#include <ge_engine/Components.h>
#include <ge_engine/Core.h>

namespace geProject {
	class Action {
	public:
		unsigned int preconditions{ 0 };
		unsigned int effects{ 0 };
		float cost{1.0f};
		float actionTime;
		bool inRange;
		Transform target;
		bool actionExecuting{ false };	
		unsigned int getEffects() { return effects; };
		unsigned int getPreconditions() { return preconditions; };
		float getCost() {return cost;};
		virtual void executeAction() = 0;
	};

	class DodgeAction : public Action {
	public:
		DodgeAction() {
			cost = 5;
			preconditions |= ATTACK_SIGHTED;
			
		};		
		
		void executeAction() {};

	};

	class ShieldAction : public Action {
	public:
		ShieldAction() {
			cost = 4;
			preconditions |= ATTACK_SIGHTED | HAS_ENERGY;
			effects |= ~HAS_ENERGY;
		};
	
		void executeAction() {};
	};

	class DashAction : public Action {
	public:
		DashAction() {
			cost = 4;
			preconditions |= ATTACK_SIGHTED | HAS_ENERGY;
			effects |= ~HAS_ENERGY;
		};
		
		void executeAction() {};
	};

	class RecoverEnergyAction : public Action {
	public:
		RecoverEnergyAction() {
			preconditions |= ~HAS_ENERGY;
			effects |= HAS_ENERGY;
		};
	
		void executeAction() {};
	};

	class PickupObjectAction : public Action {
	public:
		PickupObjectAction() {
			preconditions |= POWERUP_IN_RANGE;
			effects |= ~POWERUP_IN_RANGE;
		};
	
		void executeAction() {};
	};

	class InvestigateAction : public Action {
	public:
		InvestigateAction() {
			preconditions |= ALERT | ~AT_HOME;
			effects |= ~ALERT;
		};
	
		void executeAction() {};
	};

	class PatrolAction : public Action {
	public:
		PatrolAction() {
			preconditions |= AT_HOME  | ~ENEMY_VISIBLE | ~PATROLLED;
			effects |= PATROLLED | ~AT_HOME;
		};

		void executeAction() {};
	};

	class WaitAction : public Action {
	public:
		WaitAction() {
			preconditions |= AGENT_ANGRY;
			effects |= ~AGENT_ANGRY;
		};
		
		void executeAction() {};
	};

	class GoHomeAction : public Action {
	public:
		GoHomeAction() {
			preconditions |= PATROLLED | ~AT_HOME | ~ENEMY_VISIBLE;
			effects |= AT_HOME;
		};

		void executeAction() {};
	};

	class FireAttackAction : public Action {
	public:
		FireAttackAction() {
			preconditions |= ENEMY_VISIBLE | ~ENEMY_DEAD | FIRE_ENERGY;
			effects |= ENEMY_DEAD | ~FIRE_ENERGY;
		};

		void executeAction() {};
	};

	class WaterAttackAction : public Action {
	public:
		WaterAttackAction() {
			preconditions |= ENEMY_VISIBLE | ~ENEMY_DEAD | WATER_ENERGY;
			effects |= ENEMY_DEAD | ~WATER_ENERGY;
		};

		void executeAction() {};
	};

	class WindAttackAction : public Action {
	public:
		WindAttackAction() {
			preconditions |= ENEMY_VISIBLE | ~ENEMY_DEAD | WIND_ENERGY;
			effects |= ENEMY_DEAD | ~WIND_ENERGY;
		};

		void executeAction() {};
	};

	class EarthAttackAction : public Action {
	public:
		EarthAttackAction() {
			preconditions |= ENEMY_VISIBLE | ~ENEMY_DEAD | EARTH_ENERGY;
			effects |= ENEMY_DEAD | ~EARTH_ENERGY;
		};

		void executeAction() {};
	};




}