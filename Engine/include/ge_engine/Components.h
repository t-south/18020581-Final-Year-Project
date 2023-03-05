#pragma once
#include <glm/glm.hpp>
#include "../src/Memory/PoolAllocator.h"

namespace geProject {



	struct Transform {
		unsigned int id = 0x00000001;
		glm::vec2 position{ 0 };
		glm::vec2 scale{ 0 };
		glm::vec3 dirtyFlag{ 0,0,0 };
	};

	struct SpriteRender {
		unsigned int id = 0x00000002;
		glm::vec4 color{ 0, 0, 0, 0 };
		glm::vec2 texturePos[4] = { glm::vec2{1.0f, 1.0f}, glm::vec2{1.0f, 0.0f}, glm::vec2{0.0f, 0.0f}, glm::vec2{0.0f, 1.0f} };
		unsigned int textureId{ 0 };
		unsigned int spriteSheetId{ 0 }; // 0 == no spritesheet
		unsigned int zIndex {0};
		glm::vec3 dirtyFlag{ 0,0,0 }; // dirtyflag  -- renderbatch -- index
	};

	struct Rigidbody {
		unsigned int id = 0x00000004;
		int collider{ 0 };
		float friction{ 0.8 };
		glm::vec3 velocity{ 0, 0.5, 0 };

		glm::vec3 dirtyFlag{ 0,0,0 }; // dirtyflag  -- renderbatch -- index
	
	};

	struct FontRender {
		unsigned int id = 0x00000008;
		int x;
		int y;
		int z;
		glm::vec3 dirtyFlag{ 0,0,0 };

	};
	
	
}




