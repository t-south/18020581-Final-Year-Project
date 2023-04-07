#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../src/Memory/PoolAllocator.h"
#include <unordered_map>
#include <vector>
#include <string>

namespace geProject {
	struct Transform {
		unsigned int id = 1;
		std::string name{"object"};
		glm::vec2 position{ 0 };
		glm::vec2 scale{ 0 };
		float rotation{ 0};	
		glm::vec3 dirtyFlag{0,0,-1 };
	};

	struct SpriteRender {
		unsigned int id = 2;
		unsigned int entityId;
		glm::vec4 color{ 1, 1, 1, 1};
		glm::vec2 texturePos[4] = { glm::vec2{1.0f, 1.0f}, glm::vec2{1.0f, 0.0f}, glm::vec2{0.0f, 0.0f}, glm::vec2{0.0f, 1.0f} };
		GLuint textureId{ 0 };
		unsigned int spriteSheetId{ 0 }; // 0 == no spritesheet
		unsigned int zIndex {0}; 		
	};

	struct Rigidbody {
		unsigned int id = 4;
		int collider{ 0 };
		float friction{ 0.8f };
		float angularDamping{0.0f};
		float linearDamping{ 0.0f };
		float density{ 0.0f };
		bool fixedRotate{ false };
		bool bullet{ false };		
		unsigned int bodyType{ 1 }; // 0 -> kinematic ||| 1 -> dynamic ||| 2 -> static
		glm::vec3 velocity{ 0, 0.5, 0 };	
	};

	struct CircleCollider {
		unsigned int id = 8;
		int entityAssigned;
		float radius{ 0.0f };
		glm::vec2 offset;
		bool sensor{ false };
	};

	struct BoxCollider {
		unsigned int id = 16;		
		int entityAssigned;
		glm::vec2 boxSize{0.25f, 0.25f};
		glm::vec2 offset{ 0 };
		glm::vec2 origin{0,0};
		bool sensor{ false };

	};

	struct Animation {
		unsigned int id = 32;		
		float speed{0.8f};
		int currentFrame{ 0 };
		bool loopAnimation{ true };
		std::string state;
	};

	struct Controls {
		unsigned int id = 64;
		float runspeed{ 1.0f };
		float speedModifier;
		float direction;
	};

	struct Health {
		unsigned int id = 128;
		int maxHealth;
		int currentHealth;
		bool invincible;
	};
	

	struct Damage {
		unsigned int id = 256;
		int dmgAtk;
		int dmgModifier;
		int dmgType;
	};
	
}




