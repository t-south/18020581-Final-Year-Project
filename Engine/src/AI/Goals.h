#pragma once
#include <unordered_map>
#include "WorldStates.h"
namespace geProject {
	class Goal {	
	public:
		std::string name;
		int goalType;
		int priority{ 10 };
		void increasePriority(int increase) { priority += increase;  if (priority > 10) { priority = 10; } };
		void decreasePriority(int decrease) { priority -= decrease; if (priority < 0) { priority = 0; } };
		virtual int checkCondition(int state) =0;
		virtual bool checkValid(int state) = 0;
		                                                                                           
	};

	class AttackEnemyGoal :public Goal {
	public:
		AttackEnemyGoal() { name = "Attack Goal"; goalType = actionType::COMBAT; };
		int checkCondition(int state) { return state | ENEMY_DEAD; };
		bool checkValid(int state) { 
			int newState = state;
			newState |= ENEMY_VISIBLE | COOLDOWN;
			newState &= ~ENEMY_DEAD ;
			return newState == state; 
		}
	
	};

	class PatrolGoal : public Goal {
	public:
		PatrolGoal() { name = "Patrol Goal"; goalType = actionType::DUTY;};
		int checkCondition(int state) {	state = state | PATROLLED;	return state & ~AT_HOME; };
		bool checkValid(int state) {
			int newState = state;
			newState |= HAS_ENERGY;
			newState &= ~PATROLLED & ~ALERT & ~ENEMY_VISIBLE;
			return newState == state; 
		};
	};

	class GoHomeGoal : public Goal {
	public:
		GoHomeGoal() { name = "GoHome Goal" ; goalType = actionType::DUTY;};
		int checkCondition(int state) { state = state | AT_HOME;	return state & ~PATROLLED; };
		bool checkValid(int state) { 
			int newState = state;
			newState |= HAS_ENERGY | PATROLLED;
			newState &= ~AT_HOME & ~ALERT & ~ENEMY_VISIBLE;
			return newState == state;
		};
	};

	class CalmDownGoal : public Goal {
	public:
		CalmDownGoal() { name = "CalmDown Goal" ; goalType = actionType::LIFESTYLE;};
		int checkCondition(int state) { return state & ~AGENT_ANGRY; };
		bool checkValid(int state) { 
			return (AGENT_ANGRY & state) == AGENT_ANGRY; 
		}
	};


	class RestGoal : public Goal {
	public:
		RestGoal() { name = "Rest Goal"; goalType = actionType::LIFESTYLE | COMBAT;};
		int checkCondition(int state) { return state | HAS_ENERGY; };
		bool checkValid(int state) { 			
			return (~HAS_ENERGY & state) == state; };
	};

	class InvestigateGoal : public Goal {
	public:
		InvestigateGoal() { name = "Investigate Goal"; goalType = actionType::DUTY;};
		int checkCondition(int state) { 
			state = state | INVESTIGATED;
			state = state & ~ALERT;
			return  state; 
		};
		bool checkValid(int state) { 
			return (ALERT & state) == ALERT; 
		};
	};

	class FleeGoal : public Goal {
	public:
		FleeGoal() { name = "Flee Goal"; };		
		int checkCondition(int state) { return  state & ~ENEMY_VISIBLE; goalType = actionType::COMBAT;};
		bool checkValid(int state) { 
			return (ENEMY_VISIBLE & state) == state; 
		};
	};

	class StayAliveGoal : public Goal {
	public:
		StayAliveGoal() { name = "Stay Alive Goal"; goalType = actionType::COMBAT;};
		int checkCondition(int state) { return state & ~AGENT_HURT; };
		bool checkValid(int state) { 
			return (AGENT_HURT & state) == state; 
		};
	};
}
