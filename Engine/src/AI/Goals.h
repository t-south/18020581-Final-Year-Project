#pragma once
#include <unordered_map>
#include "WorldStates.h"
namespace geProject {
	class Goal {	
	public:
		int priority;
		unsigned int condition;
		bool checkValid(unsigned int state) { return (state & condition) == condition; };
		unsigned int getCondition() { return condition; };
	};

	class AttackEnemyGoal :public Goal {
		AttackEnemyGoal() { condition = ENEMY_DEAD; };	
	};

	class CalmDownGoal : public Goal {	
		CalmDownGoal() { condition = ~AGENT_ANGRY; };	
	};

	class PatrolGoal : public Goal {
		PatrolGoal() { condition = PATROLLED;};
		
	};

	class RestGoal : public Goal {
		RestGoal() { condition = RESTED; };
		
	};

	class InvestigateGoal : public Goal {
		InvestigateGoal() { condition = ~ALERT; };
		
	};

	class FleeGoal : public Goal {
		FleeGoal() { condition = ~ENEMY_VISIBLE; };
	};


	class StayAliveGoal : public Goal {
		StayAliveGoal() { condition = ~AGENT_HURT; };
	};


}
