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
		int currentState;
		int goalState;
		Action* actionTaken;		
		float gValue{ 0 };
		float fValue{ 0 };
		float heuristic{ 0 };

		std::vector<asNode*> neighbours;
		asNode* parent{ nullptr };
	};
	class Planner {
	public:
		std::vector<Action*> createPlan(Goal& goal, int agentId, std::vector<Action*> actionsAvailable);
		std::vector<Action*> recreate;

	};
}