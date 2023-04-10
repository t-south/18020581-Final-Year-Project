#include "Planner.h"



std::vector<geProject::Action*> geProject::Planner::createPlan(Goal& goal, unsigned int currentState, std::vector<Action*> actionsAvailable) {		
	std::vector<Action*> actionPlan;
    std::vector<asNode*> openList;
    std::vector<asNode*> closedList;
	//find differences in current agent/world state and the goals required world state
	unsigned int goalCondition = goal.getCondition();
	asNode* startNode = new asNode();
	if ((currentState & goalCondition) != goalCondition) {
		startNode->currentState = currentState;
		startNode->goalState = currentState | goalCondition;
	}
	openList.push_back(startNode);
	asNode* expandedNode = nullptr;
	//A STAR
    while (openList.size() > 0) {
		int index = 0;
		int count = 0;
		//set the newly expanded node
		expandedNode = openList[0];
		for (auto& node : openList) {
			if (node->fValue < expandedNode->fValue) {
				expandedNode = node;
				index = count;
			}
			count++;
		}		
		//check both bools for current state and goal state are the same		
		//if both are the same then we have found the optimal path
		if (expandedNode->currentState == expandedNode->goalState) {
			//create the action plan by following the nodes parents back to the origin node
			while (expandedNode->parent != nullptr) {
				actionPlan.push_back(expandedNode->actionTaken);
				expandedNode = expandedNode->parent;
			}
			return actionPlan;
		}
		openList.erase(openList.begin() + index);
		closedList.push_back(expandedNode);

		//find adjacent nodes that fulfill the differences in effects 
		for (auto& newAction : actionsAvailable) {				
			//if action effect state matches the goal state
			if ((newAction->getEffects() & expandedNode->goalState) == newAction->getEffects()) {
				asNode* neighbour = new asNode();				
				neighbour->currentState = expandedNode->goalState;
				neighbour->actionTaken = newAction;
				unsigned int precon = newAction->getPreconditions();				
				//fill in precondition values for action 
				neighbour->currentState = neighbour->goalState;
				neighbour->goalState |= precon;
				expandedNode->neighbours.push_back(neighbour);				
			}			
		}

		for (auto& neighbour : expandedNode->neighbours) {
			//add cost to take action from current node to current accumulated action cost
			float gValue = 0;// expandedNode->gValue + neighbour->actionTaken->getCost();
			//heuristic is the number of state differences left till starting state
			int count = 0;
			for (int i = 0; i < 32; i++) {
				if (((expandedNode->goalState >> i) & 1) != ((expandedNode->currentState >> i) & 1)) {
					count++;
				}
			}
			int heuristic = count;
			
			float fValue = gValue + heuristic;
			bool neighbourclosed = std::find(closedList.begin(), closedList.end(), neighbour) != closedList.end();
			bool neighbouropen = std::find(openList.begin(), openList.end(), neighbour) != openList.end();
			if (fValue < expandedNode->fValue && neighbourclosed) {
				neighbour->fValue = fValue;
				neighbour->gValue = gValue;
				expandedNode = neighbour->parent;
			}
			else if (expandedNode->gValue < gValue && neighbouropen) {
				neighbour->fValue = fValue;
				neighbour->gValue = gValue;
				neighbour->parent = expandedNode;
			}
			else if (!neighbouropen && !neighbourclosed) {
				neighbour->fValue = fValue;
				neighbour->gValue = gValue;
				openList.push_back(neighbour);
			}
		}
    }
	return actionPlan;
	
}
