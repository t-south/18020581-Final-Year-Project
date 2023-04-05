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
		virtual void getCost() = 0;
		virtual void executeAction() = 0;
	};

	class DodgeAction : public Action {
		DodgeAction() {
			cost = 5;
			preconditions |= ATTACK_SIGHTED;
			
		};		
		void getCost() {};		
		void executeAction() {};

	};

	class ShieldAction : public Action {
		ShieldAction() {
			cost = 4;
			preconditions |= ATTACK_SIGHTED | HAS_ENERGY;
			effects |= ~HAS_ENERGY;
		};
		void getCost() {};
		void executeAction() {};
	};

	class DashAction : public Action {
		DashAction() {
			cost = 4;
			preconditions |= ATTACK_SIGHTED | HAS_ENERGY;
			effects |= ~HAS_ENERGY;
		};
		void getCost() {};
		void executeAction() {};
	};

	class RecoverEnergyAction : public Action {
		RecoverEnergyAction() {
			preconditions |= ~HAS_ENERGY;
			effects |= HAS_ENERGY;
		};
		void getCost() {};
		void executeAction() {};
	};

	class PickupObjectAction : public Action {
		PickupObjectAction() {
			preconditions |= POWERUP_IN_RANGE;
			effects |= ~POWERUP_IN_RANGE;
		};
		void getCost() {};
		void executeAction() {};
	};

	class InvestigateAction : public Action {
		InvestigateAction() {
			preconditions |= ALERT | ~AT_HOME;
			effects |= ~ALERT;
		};
		void getCost() {};
		void executeAction() {};
	};

	class PatrolAction : public Action {
		PatrolAction() {
			preconditions |= AT_HOME  | ~ENEMY_VISIBLE | ~PATROLLED;
			effects |= PATROLLED | ~AT_HOME;
		};
		void getCost() {};
		void executeAction() {};
	};

	class WaitAction : public Action {
		WaitAction() {
			preconditions |= AGENT_ANGRY;
			effects |= ~AGENT_ANGRY;
		};
		void getCost() {};
		void executeAction() {};
	};

	class GoHomeAction : public Action {
		GoHomeAction() {
			preconditions |= PATROLLED | ~AT_HOME | ~ENEMY_VISIBLE;
			effects |= AT_HOME;
		};
		void getCost() {};
		void executeAction() {};
	};

	class FireAttackAction : public Action {
		FireAttackAction() {
			preconditions |= ENEMY_VISIBLE | ~ENEMY_DEAD | FIRE_ENERGY;
			effects |= ENEMY_DEAD | ~FIRE_ENERGY;
		};
		void getCost() {};
		void executeAction() {};
	};

	class WaterAttackAction : public Action {
		WaterAttackAction() {
			preconditions |= ENEMY_VISIBLE | ~ENEMY_DEAD | WATER_ENERGY;
			effects |= ENEMY_DEAD | ~WATER_ENERGY;
		};
		void getCost() {};
		void executeAction() {};
	};

	class WindAttackAction : public Action {
		WindAttackAction() {
			preconditions |= ENEMY_VISIBLE | ~ENEMY_DEAD | WIND_ENERGY;
			effects |= ENEMY_DEAD | ~WIND_ENERGY;
		};
		void getCost() {};
		void executeAction() {};
	};

	class EarthAttackAction : public Action {
		EarthAttackAction() {
			preconditions |= ENEMY_VISIBLE | ~ENEMY_DEAD | EARTH_ENERGY;
			effects |= ENEMY_DEAD | ~EARTH_ENERGY;
		};
		void getCost() {};
		void executeAction() {};
	};




}