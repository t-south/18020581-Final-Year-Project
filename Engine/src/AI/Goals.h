#pragma once
#include <unordered_map>
#include "WorldStates.h"
namespace geProject {
	class Goal {	
	public:
		std::string name;
		int goalType;
		int priority{ 10 };
		void increasePriority() { if (priority < 10) { priority++; } };
		void decreasePriority() { if (priority > 0) {priority--;} };
		virtual int checkCondition(int state) =0;
		virtual bool checkValid(int state) = 0;
		                                                                                           
	};

	class AttackEnemyGoal :public Goal {
	public:
		AttackEnemyGoal() { name = "Attack Goal"; goalType = actionType::COMBAT; };
		int checkCondition(int state) { return state | ENEMY_DEAD; };
		bool checkValid(int state) { 
			return ((~ENEMY_DEAD & state) | (ENEMY_VISIBLE | state)) == state; 
		}
	
	};

	class PatrolGoal : public Goal {
	public:
		PatrolGoal() { name = "Patrol Goal"; goalType = actionType::DUTY;};
		int checkCondition(int state) {	state = state | PATROLLED;	return state & ~AT_HOME; };
		bool checkValid(int state) {
			return ((HAS_ENERGY | state) & (AT_HOME | state) & (~PATROLLED & state)) == state; 
		};
	};

	class GoHomeGoal : public Goal {
	public:
		GoHomeGoal() { name = "GoHome Goal" ; goalType = actionType::DUTY;};
		int checkCondition(int state) { state = state | AT_HOME;	return state & ~PATROLLED; };
		bool checkValid(int state) { 
			int newState = state;
			newState |= HAS_ENERGY | PATROLLED;
			newState &= ~AT_HOME;
			return newState == state;
		};
	};

	class CalmDownGoal : public Goal {
	public:
		CalmDownGoal() { name = "CalmDown Goal" ; goalType = actionType::LIFESTYLE;};
		int checkCondition(int state) { return state & ~AGENT_ANGRY; };
		bool checkValid(int state) { 
			return (AGENT_ANGRY & state) == state; 
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
		InvestigateGoal() { name = "Investigate Goal"; goalType = actionType::COMBAT;};
		int checkCondition(int state) { return  state & ~ALERT; };
		bool checkValid(int state) { 
			return (ALERT & state) == state; 
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
