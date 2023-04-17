#pragma once
#include <unordered_map>
#include <string>
#include <ge_engine/Components.h>
#include <ge_engine/Core.h>
#include <ge_engine/Managers.h>
#include <ge_engine/PhysicsManager.h>
#include "../Controller/Command.h"
#include "../Controller/EnemyController.h"

namespace geProject {
	enum actionType {
		COMBAT, LIFESTYLE, DUTY
	};

	class Action {
	public:
		float cost;
		float actionTime;
		actionType actiontype;
		bool inRange{ false };
		bool completed{ false };
		Transform target{.id = 0};	
		bool attack{ false };
		bool rotate{ false };
		float dmgModifier;
		int cooldown;

		int attackType;	
		int agentId;
		float getCost() {return cost;};
		bool isComplete() { return completed; };
		bool isValid() {}; 
		void updateAgentEnergy(float energy) {
			Agent newagent = entitymanager.getAgentComponent(agentId);
			newagent.energy += energy;
			if(newagent.energy < -20){
				newagent.energy = -20;
			}
			entitymanager.assignAgent(agentId, newagent);
		};
		virtual void executeAction(float posx, float posy) = 0;
		virtual int setEffect(int state) = 0;
		void updateEffect(bool add, int newstates) {
			int state = entitymanager.getAgentState(agentId);
			if (add) {
				state = state | newstates;
			}
			else {
				state = state & newstates;
			}			
			entitymanager.updateAgentState(agentId, state);
		};
		virtual bool checkPrecondition(int state) =0;
		virtual int applyPrecondition(int state) = 0;
		virtual bool proceduralPrecondition(int agentId) = 0;
		
	};

	class DodgeAction : public Action {
	public:
		DodgeAction(int id) { actiontype = COMBAT; agentId = id; cost = 1; };
		void executeAction(float posx, float posy) { 

	
		};
		int setEffect(int state) {
			return state;
		};
		bool checkPrecondition(int state) { int newState = state; newState |= ATTACK_SIGHTED; return newState == state; };
		int applyPrecondition(int state) { state |= ATTACK_SIGHTED; return state; };
		bool proceduralPrecondition(int agentId) {
			bool targetFound = true;
			inRange = true;

			Transform playerpos = entitymanager.getTransformComponent(entitymanager.getPlayerId());
			Transform agentpos = entitymanager.getTransformComponent(agentId);
			if (physicsmanager.checkTargetObstructed(agentpos.position.x, agentpos.position.y, playerpos.position.x, playerpos.position.y)) {
				target = playerpos;
				targetFound = false;
				inRange = true;
			}
			if (targetFound) {
				updateEffect(true, ENEMY_VISIBLE);
				updateEffect(false, ~ENEMY_DEAD);
			}
			else { updateEffect(false, ~ENEMY_VISIBLE); }
			return targetFound;
			
		
		};
	
	};



	class RecoverEnergyAction : public Action {
	public:
		RecoverEnergyAction(int id) {actiontype = COMBAT; agentId = id; cost = 1;
		};
	
		void executeAction(float posx, float posy) {
			//cost = -1; 
			Agent agent = entitymanager.getAgentComponent(agentId);
			completed = true; 
			updateAgentEnergy(0.5f);
			if (agent.energy > 10) {
				updateEffect(true, HAS_ENERGY);
			}
	
		};

		int setEffect(int state) {
			state = state | HAS_ENERGY;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState &= ~HAS_ENERGY; return newState == state; };
		int applyPrecondition(int state) { state &= ~HAS_ENERGY; return state; };
		bool proceduralPrecondition(int agentId) {
			return true;
		};

	};

	class InvestigateAction : public Action {
	public:
		InvestigateAction(int id) {actiontype = COMBAT; agentId = id; cost = 1;
		};
		void executeAction(float posx, float posy) {
			if (inRange) {
				completed = true;
				inRange = false;
				target.id = 0;	
				Agent agent = entitymanager.getAgentComponent(agentId);
				if (agent.alertLevel == 0) {
					updateEffect(false, ~ALERT);
					updateEffect(true, INVESTIGATED);
			
					
				}
				else {
					agent.alertLevel -= 5;
					entitymanager.assignAgent(agentId, agent);
				}
						
				updateAgentEnergy(-cost);
				cost = 1;
			}		
			
			float distance = worldstate.calculateEuclidean(posx, posy, target.position.x, target.position.y);
			cost = cost * 10 * distance;
		
		};

		int setEffect(int state) {
			state = state | INVESTIGATED;
			state = state & ~ALERT;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= ALERT | HAS_ENERGY; newState &= ~AT_HOME & INVESTIGATED; return newState == state; };
		int applyPrecondition(int state) { state |= ALERT | HAS_ENERGY ; state &= ~AT_HOME; return state; };
		bool proceduralPrecondition(int agentId) { 		
			bool anomalyFound = false;		
			Agent agent = entitymanager.getAgentComponent(agentId);
			Transform agentpos = entitymanager.getTransformComponent(agentId);
			if (worldstate.calculateEuclidean(agentpos.position.x, agentpos.position.y, agent.anomalypos.x, agent.anomalypos.y) < 5 && worldstate.getTile(std::round(agent.anomalypos.x * 4), std::round(agent.anomalypos.y * 4))) {
				agentpos.position = agent.anomalypos;
				target = agentpos;			
				anomalyFound = true;
			}	
		
			return anomalyFound;
		
		};

	};

	class PatrolAction : public Action {
	public:
		PatrolAction(int id) {actiontype = DUTY; agentId = id; cost = 1;
		};
		
		void executeAction(float posx, float posy) {			
			updateEffect(false, ~AT_HOME);
			if (inRange) {	
				updateEffect(true, PATROLLED);
				updateAgentEnergy(-cost);
				completed = true;
				inRange = false;
				target.id = 0;	
				cost = 1;
			}
			else if (target.id == 0) {
				target = worldstate.getRandomPoint(posx, posy);
			}
			float distance = worldstate.calculateEuclidean(posx, posy, target.position.x, target.position.y);
			cost = cost * 10 * distance;
			
		};

		int setEffect(int state) {
			state = state | PATROLLED;
			state = state & ~AT_HOME;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= AT_HOME | PATROLLED; newState &= ~ENEMY_VISIBLE & ~PATROLLED; return newState == state; };
		int applyPrecondition(int state) { state |= AT_HOME; state &= ~ENEMY_VISIBLE & ~PATROLLED; return state; };
		bool proceduralPrecondition(int agentId) { return true; };

	};

	class WaitAction : public Action {
	public:
		WaitAction(int id) {actiontype = LIFESTYLE; agentId = id; cost = 1;
		};
		void executeAction(float posx, float posy) { 	
			updateAgentEnergy(-cost);};

		int setEffect(int state) {
			state = state & ~AGENT_ANGRY;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= AGENT_ANGRY; return newState == state; };
		int applyPrecondition(int state) { state |= AGENT_ANGRY; return state; };
		bool proceduralPrecondition(int agentId) { return true; };
	
	};

	class GoHomeAction : public Action {
	public:
		GoHomeAction(float posX, float posY, int id) {actiontype = LIFESTYLE;target = Transform{ .position = glm::vec2(posX, posY) }; agentId = id;	cost = 1;
		};

		void executeAction(float posx, float posy) {					
			if (inRange || (posx == target.position.x  && posy == target.position.y)) {
				updateEffect(true, AT_HOME);
				updateEffect(false, ~PATROLLED);
				updateAgentEnergy(-cost);
				completed = true;
				inRange = false;	
				cost = 1;
			}
		
			float distance = worldstate.calculateEuclidean(posx, posy, target.position.x, target.position.y);
			cost = cost * 10 * distance;
			
		};

		int setEffect(int state) {
			state = state | AT_HOME;	
			state = state & ~PATROLLED;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= PATROLLED; newState &= ~ENEMY_VISIBLE & ~AT_HOME; return newState == state; };
		int applyPrecondition(int state) { state |= PATROLLED; state &= ~ENEMY_VISIBLE & ~AT_HOME; return state; };
		bool proceduralPrecondition(int agentId) { return true; };

	};

	class FireAttackAction : public Action {
	public:
		FireAttackAction(int id) {
			actiontype = COMBAT; agentId = id; cost = 1; cooldown = 5; dmgModifier = 1.2f;
		};

		void executeAction(float posx, float posy) {

			attack = true;			
			attackType = dmgTypes::FIRE;
			completed = true;
			updateEffect(false, ~COOLDOWN);
			updateAgentEnergy(-cost );
		};
	

		int setEffect(int state) {
			state = state | ENEMY_DEAD;
			state = state & ~COOLDOWN;	
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= ENEMY_VISIBLE | COOLDOWN; newState &= ~ENEMY_DEAD ; return newState == state; };

		int applyPrecondition(int state) { 			
			state |= ENEMY_VISIBLE | COOLDOWN;
			state &= ~ENEMY_DEAD ; 
			return state; 
		
		};

		bool proceduralPrecondition(int agentId) {
			bool targetFound = true;
			inRange = true;
			
			Transform playerpos = entitymanager.getTransformComponent(entitymanager.getPlayerId());
			Transform agentpos = entitymanager.getTransformComponent(agentId);		
			if (physicsmanager.checkTargetObstructed(agentpos.position.x, agentpos.position.y, playerpos.position.x, playerpos.position.y)) {
				target = playerpos;
				targetFound = false;
				inRange = true;
			}	
			if (targetFound) {
				updateEffect(true, ENEMY_VISIBLE);
				updateEffect(false, ~ENEMY_DEAD);
			}
			else { updateEffect(false, ~ENEMY_VISIBLE); }
			return targetFound;
		};


	};

	class WaterAttackAction : public Action {
	public:
		WaterAttackAction(int id) { actiontype = COMBAT; agentId = id; cost = 1; cooldown = 1; dmgModifier = 0.5f; };

		void executeAction(float posx, float posy) {
			attack = true;
			attackType = dmgTypes::WATER;
			completed = true;		
			updateEffect(true, COOLDOWN);
			updateAgentEnergy(-cost);
			//updateEffect(true, ENEMY_DEAD);
		};

		int setEffect(int state) {
			state = state | ENEMY_DEAD | COOLDOWN;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= ENEMY_VISIBLE; newState &= ~ENEMY_DEAD & ~COOLDOWN; return newState == state; };
		int applyPrecondition(int state) { 
			state |= ENEMY_VISIBLE; 
			state &= ~ENEMY_DEAD & ~COOLDOWN;
			return state; };

		bool proceduralPrecondition(int agentId) { 
			bool targetFound = true;
			inRange = true;
			Transform playerpos = entitymanager.getTransformComponent(entitymanager.getPlayerId());
			Transform agentpos = entitymanager.getTransformComponent(agentId);
			if (physicsmanager.checkTargetObstructed(agentpos.position.x, agentpos.position.y, playerpos.position.x, playerpos.position.y)) {
				target = playerpos;
				targetFound = false;
				inRange = false;
			}
			if (targetFound) {
				updateEffect(true, ENEMY_VISIBLE);
				updateEffect(false, ~ENEMY_DEAD);
			}
			else { updateEffect(false, ~ENEMY_VISIBLE); }
			return targetFound;
		};


	};

	class WindAttackAction : public Action {
	public:
		WindAttackAction(int id) {
			actiontype = COMBAT; agentId = id; cost = 1; dmgModifier = 3;
		};

		void executeAction(float posx, float posy) { 			
			attack = true;
			attackType = dmgTypes::LIGHTNING;			
			completed = true;
			updateEffect(true, COOLDOWN);
			//updateEffect(true, ENEMY_DEAD);	
			updateAgentEnergy(-cost);
		};


		int setEffect(int state) {
			state = state | ENEMY_DEAD | COOLDOWN;
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= ENEMY_VISIBLE; newState &= ~ENEMY_DEAD & ~COOLDOWN; return newState == state; };
		
		int applyPrecondition(int state) { 
			state |= ENEMY_VISIBLE; 
			state &= ~ENEMY_DEAD & ~COOLDOWN;
			return state; };

		bool proceduralPrecondition(int agentId) {
			bool targetFound = true;
			inRange = true;
			Transform playerpos = entitymanager.getTransformComponent(entitymanager.getPlayerId());
			Transform agentpos = entitymanager.getTransformComponent(agentId);
			if (physicsmanager.checkTargetObstructed(agentpos.position.x, agentpos.position.y, playerpos.position.x, playerpos.position.y)) {
				target = playerpos;
				targetFound = false;
				inRange = false;
			}
			if (targetFound) {
				updateEffect(true, ENEMY_VISIBLE);
				updateEffect(false, ~ENEMY_DEAD);
			}
			else { updateEffect(false, ~ENEMY_VISIBLE); }
			return targetFound;
		};

	};

	class EarthAttackAction : public Action {
	public:
		EarthAttackAction(int id) {
			actiontype = COMBAT; agentId = id; cost = 1; dmgModifier = 1;
		};

		void executeAction(float posx, float posy) { 
		
			attack = true;
			attackType = dmgTypes::EARTH;
			completed = true;
			updateEffect(true, COOLDOWN);
			//updateEffect(true, ENEMY_DEAD);
			updateAgentEnergy(-cost);
		};

		int setEffect(int state) {
			state = state | ENEMY_DEAD | COOLDOWN;			
			return state;
		};

		bool checkPrecondition(int state) { int newState = state; newState |= ENEMY_VISIBLE ; newState &= ~ENEMY_DEAD & ~COOLDOWN; return newState == state; };
		
		int applyPrecondition(int state) { 
			state |= ENEMY_VISIBLE ; 
			state &= ~ENEMY_DEAD & ~COOLDOWN;
			return state; };

		bool proceduralPrecondition(int agentId) { 
			bool targetFound = true;
			inRange = true;
			Transform playerpos = entitymanager.getTransformComponent(entitymanager.getPlayerId());
			Transform agentpos = entitymanager.getTransformComponent(agentId);		
			if (physicsmanager.checkTargetObstructed(agentpos.position.x, agentpos.position.y, playerpos.position.x, playerpos.position.y)) {
				target = playerpos;
				targetFound = false;				
				inRange = false;
			}		
			if (targetFound) {
				updateEffect(true, ENEMY_VISIBLE);
				updateEffect(false, ~ENEMY_DEAD);
			}
			else{ updateEffect(false, ~ENEMY_VISIBLE); }
			return targetFound;
		};

	};




}