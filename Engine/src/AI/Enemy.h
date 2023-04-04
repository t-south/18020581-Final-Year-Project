#pragma once
#include <vector>
#include "Action.h"
#include "Goals.h"
#include "Planner.h"

namespace geProject {
	enum State {
		IDLE, MOVE, ACTION
	}

	class Enemy {
	public:
		Enemy();
		void update(float deltaTime);
	private:
		static Planner actionPlanner;
		glm::vec2 position;

		//STATE TRANSITIONS  ||  IDLE -- ACTION -- MOVE
		State currentState{State::IDLE};		

		//GOALS
		Goal currentGoal;
		std::vector<Goal> availableGoals;

		//ACTIONS
		std::vector<Action> actionsAvailable;
		std::vector<Action> actionPlan;

				 
		void executeAction();
		Goal chooseGoal();
		void addGoal(Goal& goal);
		void removeGoal(Goal& goal);
		void moveState();
		void idleState();
		void actionState();
	};
