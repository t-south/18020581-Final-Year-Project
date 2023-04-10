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

namespace geProject {
	enum State {
		IDLE, MOVE, ACTION
	};

	struct pathNode {
		// pair of bools are used to check the same value for both goal values and current values
		float x;
		float y;
		float gValue{ 0 };
		float fValue{ 0 };
		float heuristic{ 0 };
		std::vector<pathNode> neighbours;
		pathNode* parent{ nullptr };
		friend bool operator==(const pathNode& lhs, const pathNode& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
	};



	
	class Enemy {
	public:
		Enemy(int entityId);
		void update(float deltaTime);
		std::vector<pathNode> planPath(float originX, float originY, float destinationX, float destinationY);
		std::vector<pathNode> getPath();
		int getPathSize();
		int getEnemyId();
	private:
		static Planner actionPlanner;
		glm::vec2 position;
		glm::vec2 oldposition;		
		float currentdirection;
		float desiredirection;
		//STATE TRANSITIONS  ||  IDLE -- ACTION -- MOVE
		State currentState{State::IDLE};		
		int agentStateDetails{ AT_HOME | FIRE_ENERGY | WATER_ENERGY | EARTH_ENERGY | WIND_ENERGY | HAS_ENERGY };
		//GOALS
		Goal currentGoal;
		std::vector<Goal> availableGoals;

		//ACTIONS
		std::vector<Action*> actionsAvailable;
		std::vector<Action*> actionPlan;
		int entityId;
		int fireEnergy;
		int waterEnergy;
		int earthEnergy;
		int windEnergy;	
		
		void executeAction();
		void setDesiredDirection();
		void moveAgent(float x, float y, float dt);
		void rotateAgent();
		void chooseGoal();
		void addGoal(Goal& goal);
		void removeGoal(Goal& goal);
		float calculateEuclidean(float originx, float originy, float destx, float desty);
		void orientAgent();
		std::vector<pathNode> path;
		std::queue<Command*> commandQueue;
		EnemyController* aiController;
	};
}