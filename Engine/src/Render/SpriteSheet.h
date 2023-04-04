#pragma once
#include "Texture.h"
#include <vector>
#include <ge_engine/Components.h>
#include <glm/glm.hpp>
namespace geProject {
	class SpriteSheet : public Texture {

	public:
		SpriteSheet(const char* filePath, unsigned int spriteNum, float spriteWidth, float spriteHeight, float borderspacing , int zIndex);
		~SpriteSheet();
		SpriteRender& getSprite(unsigned int spriteAtlasId);
		unsigned int getSpriteSize();
		unsigned int getSpriteWidth();
		unsigned int getSpriteHeight();
		unsigned int getSpriteSheetHeight();
		unsigned int getSpriteSheetWidth();
		
	private:
		std::vector<SpriteRender> sprites;
		unsigned int spriteNum, spriteHeight, spriteWidth, border, sSheetHeight, sSheetWidth;
	};
}