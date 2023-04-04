#pragma once 

namespace geProject {
	enum WorldStates {
		ENEMY_VISIBLE    = 0x00001,
		ENEMY_DEAD       = 0x00002,
		ATTACK_SIGHTED   = 0x00004,
		RESTED           = 0x00008,
		ALERT            = 0x00010,
		AGENT_ANGRY      = 0x00020,
		AGENT_TIRED      = 0x00040,
		AGENT_HURT       = 0x00080,
		PATROLLED        = 0x00100,
		HAS_ENERGY       = 0x00200,
		POWERUP_IN_RANGE = 0x00400,
		AT_HOME          = 0x00800,
		FIRE_ENERGY      = 0x01000,
		WATER_ENERGY     = 0x02000,
		EARTH_ENERGY     = 0x04000,
		WIND_ENERGY      = 0x08000
	};

	struct WorldNode {
		float x, y;		
		float gValue{ 0 };
		float fValue{ 0 };
		float heuristic{ 0 };

		std::vector<WorldNode*> neighbours;
		WorldNode* parent{ nullptr };
	};

	class WorldState {
	public:
		unsigned int worldState{ 0x00000 };
		int fireEnergy;
		int waterEnergy;
		int earthEnergy;
		int windEnergy;
		unsigned int getWorldState() { return worldState; };
		void updateWorldState(unsigned int newState) { worldState = worldState & newState; };
		void addToMap(float x, float y){}
	};
}