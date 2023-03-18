#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../src/Memory/PoolAllocator.h"

namespace geProject {
	struct Transform {
		unsigned int id = 1;
		glm::vec2 position{ 0 };
		glm::vec2 scale{ 0 };
		int rotation{ 0};
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
		float friction{ 0.8 };
		float angularDamping{0.0f};
		float linearDamping{ 0.0f };
		float density{ 0.0f };
		bool fixedRotate{ false };
		bool bullet{ false };
		unsigned int bodyType{ 0 }; // 0 -> kinematic ||| 1 -> dynamic ||| 2 -> static
		glm::vec3 velocity{ 0, 0.5, 0 };	
	};

	struct CircleCollider {
		unsigned int id = 8;
		glm::vec2 centre;
		float radius{ 0.0f };
		glm::vec2 offset;
	};

	struct BoxCollider {
		unsigned int id = 16;
		glm::vec2 centre;
		glm::vec2 offset;
		glm::vec2 origin;
	};


	struct FontRender {
		unsigned int id = 32;
		int x;
		int y;
		int z;
		glm::vec3 dirtyFlag{ 0,0,0 };

	};
	
	
}




