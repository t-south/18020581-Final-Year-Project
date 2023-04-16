#pragma once
#include <ge_engine/Core.h>
#include <vector>
#include "Action.h"
#include "Goals.h"
#include "Planner.h"
#include <queue>
#include "../Controller/Command.h"
#include "../Controller/ConcreteCommands.h"
#include "../Controller/EnemyController.h"
#include <ge_engine/Managers.h>
#include <cmath>
#include <math.h>
#include <thread>

namespace geProject {
	enum State {
		IDLE, MOVE, ACTION
	};
	
	class Enemy {
	public:
		Enemy(int entityId, bool goap);
		void update(float deltaTime);	
		std::vector<pathNode> getPath();
		int getPathSize();
		int getEnemyId();
	
	private:
		static Planner actionPlanner;
		bool aiActive;
		//STATE TRANSITIONS  ||  IDLE -- ACTION -- MOVE
		State currentState{State::IDLE};
		//GOALS
		Goal* currentGoal;
		std::vector<Goal*> availableGoals;
		//ACTIONS
		std::vector<Action*> actionsAvailable;
		std::vector<Action*> actionPlan;
		//AGENT DETAILS
		int entityId;	
		int cooldown = 5;
		glm::vec2 position;
		glm::vec2 oldposition;
		glm::vec2 homelocation;
		float currentdirection;
		float desiredirection;	
		void moveAgent(float x, float y, float dt);
		void rotateAgent();
		void chooseGoal(int agentState);
		void addGoal(Goal* goal);
		void orientAgent();
		std::vector<pathNode> path;
		std::queue<Command*> commandQueue;
		EnemyController* aiController;
		void updateActionCost(int agentState);
		void updateAgentVisibility();
	};
}