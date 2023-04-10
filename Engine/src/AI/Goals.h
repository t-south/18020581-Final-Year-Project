#pragma once
#include <unordered_map>
#include "WorldStates.h"
namespace geProject {
	class Goal {	
	public:
		int priority{ 10 };			
		void increasePriority() { if (priority < 10) { priority++; } };
		void decreasePriority() { if (priority > 0) {priority--;} };
		virtual int checkCondition(int state) =0;
		virtual bool checkValid(int state) = 0;
	};

	class AttackEnemyGoal :public Goal {
	public:
		AttackEnemyGoal() {};	
		int checkCondition(int state) { return state | ENEMY_DEAD; };
		bool checkValid(int state) { return (~ENEMY_DEAD & state) == state; }
	
	};

	class CalmDownGoal : public Goal {	
	public:
		CalmDownGoal() {};	
		int checkCondition(int state) { return state & ~AGENT_ANGRY; };
		bool checkValid(int state) { return (AGENT_ANGRY & state) == state; }
	};

	class PatrolGoal : public Goal {
	public:
		PatrolGoal() {};
		int checkCondition(int state) {	state = state | PATROLLED;	return state & ~AT_HOME; };
		bool checkValid(int state) { return (AT_HOME & ~PATROLLED & state) == state; };
	};

	class GoHomeGoal : public Goal {
	public:
		GoHomeGoal() {};
		int checkCondition(int state) { state = state | AT_HOME;	return state & ~PATROLLED; };
		bool checkValid(int state) { return (~AT_HOME & PATROLLED & state) == state; };
	};

	class RestGoal : public Goal {
	public:
		RestGoal() {};
		int checkCondition(int state) { return state | RESTED; };
		bool checkValid(int state) { return (~RESTED & state) == state; };
	};

	class InvestigateGoal : public Goal {
	public:
		InvestigateGoal() {};
		int checkCondition(int state) { return  state & ~ALERT; };
		bool checkValid(int state) { return (ALERT & state) == state; };
	};

	class FleeGoal : public Goal {
	public:
		FleeGoal() {};
		int checkCondition(int state) { return  state & ~ENEMY_VISIBLE; };
		bool checkValid(int state) { return (ENEMY_VISIBLE & state) == state; };
	};

	class StayAliveGoal : public Goal {
	public:
		StayAliveGoal() {};
		int checkCondition(int state) { return state & ~AGENT_HURT; };
		bool checkValid(int state) { return (AGENT_HURT & state) == state; };
	};
}
