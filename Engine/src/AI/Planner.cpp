#include "Planner.h"



std::vector<geProject::Action*> geProject::Planner::createPlan(Goal& goal,  int agentId, std::vector<Action*> actionsAvailable) {	
	int currentState = entitymanager.getAgentState(agentId);
	std::vector<Action*> actionPlan;
    std::vector<asNode*> openList;
    std::vector<asNode*> closedList;
	//find differences in current agent/world state and the goals required world state
	int goalCondition = goal.checkCondition(0);
	asNode* startNode = new asNode();
	
	startNode->currentState = currentState;
	startNode->goalState = goalCondition;
	
	openList.push_back(startNode);
	asNode* expandedNode = nullptr;
	//A STAR
	int track = 0;
    while (openList.size() > 0) {
		track++;
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
		//std::cout << (expandedNode->goalState & expandedNode->currentState) << std::endl;
		if ((expandedNode->goalState & expandedNode->currentState) == expandedNode->goalState) {
			//create the action plan by following the nodes parents back to the origin node
			while (expandedNode->parent != nullptr) {
				actionPlan.push_back(expandedNode->actionTaken);
				expandedNode = expandedNode->parent;
			}
			//std::cout << "action A*: " << track << std::endl;
			return actionPlan;
		}
		openList.erase(openList.begin() + index);
		closedList.push_back(expandedNode);

		//find adjacent nodes that fulfill the differences in effects 
		for (auto& newAction : actionsAvailable) {				
			//if action effect state matches the goal state
			
			if ((newAction->setEffect(0)) == expandedNode->goalState && newAction->proceduralPrecondition(agentId)) {
				asNode* neighbour = new asNode();				
				neighbour->currentState = expandedNode->goalState;
				neighbour->actionTaken = newAction;		
				//fill in precondition values for action 
				neighbour->currentState = expandedNode->currentState | expandedNode->goalState;
				neighbour->goalState =  newAction->applyPrecondition(expandedNode->goalState);
				neighbour->parent = expandedNode;
				expandedNode->neighbours.push_back(neighbour);				
			}			
		}

		for (auto& neighbour : expandedNode->neighbours) {
			//add cost to take action from current node to current accumulated action cost
			float gValue = expandedNode->gValue + neighbour->actionTaken->getCost();
			//heuristic is the number of state differences left till starting state
			int count = 0;
			//check number of differences in bits to find heuristic
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
