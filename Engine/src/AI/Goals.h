#pragma once
#include <unordered_map>
#include "WorldStates.h"
namespace geProject {
	class Goal {	
	public:
		int priority{ 10 };
		unsigned int condition;
		bool checkValid(unsigned int state) { return (state & condition) == condition; };
		unsigned int getCondition() { return condition; };
	};

	class AttackEnemyGoal :public Goal {
	public:
		AttackEnemyGoal() { condition = ENEMY_DEAD; };	
	};

	class CalmDownGoal : public Goal {	
	public:
		CalmDownGoal() { condition = ~AGENT_ANGRY; };	
	};

	class PatrolGoal : public Goal {
	public:
		PatrolGoal() { condition = PATROLLED | ~AT_HOME;};
		
	};

	class RestGoal : public Goal {
	public:
		RestGoal() { condition = RESTED; };
		
	};

	class InvestigateGoal : public Goal {
	public:
		InvestigateGoal() { condition = ~ALERT; };
		
	};

	class FleeGoal : public Goal {
	public:
		FleeGoal() { condition = ~ENEMY_VISIBLE; };
	};


	class StayAliveGoal : public Goal {
	public:
		StayAliveGoal() { condition = ~AGENT_HURT; };
	};


}
