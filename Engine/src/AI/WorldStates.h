#pragma once 
#include <ge_engine/Components.h>
#include <vector>
#include <iostream>
#include <random>
namespace geProject {


	enum AgentStates {
		AT_HOME =			0x00001,
		PATROLLED =			0x00002,
		HAS_ENERGY =		0x00004,		
		RESTED =			0x00008,
		ALERT =				0x00010,
		AGENT_ANGRY =		0x00020,
		AGENT_TIRED =		0x00040,
		AGENT_HURT =		0x00080,		
		ATTACK_SIGHTED =	0x00100,
		POWERUP_IN_RANGE =	0x00200,
		ENEMY_DEAD =		0x00400,
		ENEMY_VISIBLE =		0x00800,
		FIRE_ENERGY =		0x01000,
		WATER_ENERGY =		0x02000,
		EARTH_ENERGY =		0x04000,
		WIND_ENERGY =		0x08000
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

	class WorldState {
	public:		
		unsigned int worldState{ 0x00000 };
		int worldWidth;
		int worldHeight;
		std::vector<std::vector<int>> worldMap;
		void addToMap(int x, int y, int accessible);
		void generateWorldMap(int worldWidth, int worldHeight);
		unsigned int getWorldState();
		void updateWorldState(unsigned int newState);
		void testWorldMap();
		bool getTile(float x, float y);
		std::vector<pathNode> planPath(float originX, float originY, float destinationX, float destinationY);
		float calculateEuclidean(float originx, float originy, float destx, float desty);
		Transform getRandomPoint(float posx, float posy);
	};
}