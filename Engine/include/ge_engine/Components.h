#pragma once
#include <glm/glm.hpp>
#include "../src/Memory/PoolAllocator.h"
namespace geProject {



	struct Transform {
		static const unsigned int id = 0x00000001;
		glm::vec2 position{ 0 };
		glm::vec2 scale{ 0 };
	};

	struct SpriteRender {
		static const unsigned int id = 0x00000002;
		glm::vec4 color{ 0 };
	};

	struct FontRender {
		static const unsigned int id = 0x00000004;
		int x;
		int y;
		int z;

	};
	

}




