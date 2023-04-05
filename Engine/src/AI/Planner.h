#pragma once 
#include <vector>
#include "Action.h"
#include "Goals.h"
#include <string>
#include <unordered_map>
#include "WorldStates.h"


namespace geProject {
	struct asNode {
		// pair of bools are used to check the same value for both goal values and current values
		unsigned int currentState;
		unsigned int goalState;
		Action* actionTaken{ nullptr };		
		float gValue{ 0 };
		float fValue{ 0 };
		float heuristic{ 0 };

		std::vector<asNode*> neighbours;
		asNode* parent{ nullptr };
	};
	class Planner {
	public:
		std::vector<Action> createPlan(Goal& goal, unsigned int agentBeliefState, std::vector<Action> actionsAvailable);
		std::vector<Action> recreate;

	};
}