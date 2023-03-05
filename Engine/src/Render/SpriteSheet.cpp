#include "SpriteSheet.h"

geProject::SpriteSheet::SpriteSheet(const char* filePath, unsigned int spriteNum, float spriteWidth, float spriteHeight, float borderspacing, int zIndex) : Texture(filePath), spriteNum(spriteNum), spriteWidth(spriteWidth), spriteHeight(spriteHeight){
	//currently only designed for uniform sprites in a spritesheet
	//UV tex coords for sprite within spritesheet
	
	float x, y;
	
	x = 0;
	y = 1;
	for (int i = 0; i < spriteNum; i++) {
		SpriteRender textureUv = SpriteRender();
		textureUv.texturePos[0] = glm::vec2(x + (spriteWidth / textureWidth), y);										//topRight
		textureUv.texturePos[1] = glm::vec2(x + (spriteWidth / textureWidth),  y - (spriteHeight / textureHeight));		//bottomRight 
		textureUv.texturePos[2] = glm::vec2(x , y - (spriteHeight / textureHeight));									//bottomLeft
		textureUv.texturePos[3] = glm::vec2(x , y);																		//topLeft
		//move along x coordinate until it reaches end of row
		if (i == 13){
			float test = x + spriteWidth + borderspacing;
		}
		if (x + (spriteWidth / textureWidth) + borderspacing >= 1) {
			//set x back to beginning of spritesheet and move y down to next row
			x = 0;
			y -= ((spriteHeight / textureHeight ) + borderspacing);
		}
		else {
			x += (spriteWidth / textureWidth) + borderspacing;
		}
		textureUv.textureId = getTextureId();
		textureUv.spriteSheetId = sprites.size() + 1;
		textureUv.zIndex = zIndex;
		sprites.push_back(textureUv);
	}
	/*			
(0,1)	|			|			|			| (1,1)
	---------------------------------------------
		|			|			|			|		
		|			|			|			|
		|			|			|			|
(0,0.5)------------------------------------------(1,0.5)
		|			|			|			|
		|			|			|			|
		|			|			|			|
	---------------------------------------------
(0,0)	|			|			|			| (1,0)
	*/
}


geProject::SpriteSheet::~SpriteSheet() {
}

geProject::SpriteRender geProject::SpriteSheet::getSprite(unsigned int atlasId){
	return sprites[static_cast<std::vector<geProject::SpriteRender, std::allocator<geProject::SpriteRender>>::size_type>(atlasId) - 1];
}