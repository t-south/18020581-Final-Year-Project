#pragma once 
#include <ge_engine/Components.h>
#include <vector>
#include <iostream>
#include <random>
namespace geProject {

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