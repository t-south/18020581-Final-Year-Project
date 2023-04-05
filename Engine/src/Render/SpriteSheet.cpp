#include "SpriteSheet.h"

geProject::SpriteSheet::SpriteSheet(const char* filePath, unsigned int spriteNum, float spriteWidth, float spriteHeight, float borderspacing, int zIndex) : Texture(filePath), spriteNum(spriteNum), spriteWidth(spriteWidth), spriteHeight(spriteHeight), border(borderspacing){
	//currently only designed for uniform sprites in a spritesheet
	//UV tex coords for sprite within spritesheet
	float x, y;
	
	x = 0;
	y = 1;
	int height = 0;	
	for (int i = 0; i < spriteNum; i++) {
		SpriteRender textureUv = SpriteRender();
		textureUv.texturePos[0] = glm::vec2(x + (spriteWidth / textureWidth), y);										//topRight
		textureUv.texturePos[1] = glm::vec2(x + (spriteWidth / textureWidth),  y - (spriteHeight / textureHeight));		//bottomRight 
		textureUv.texturePos[2] = glm::vec2(x , y - (spriteHeight / textureHeight));									//bottomLeft
		textureUv.texturePos[3] = glm::vec2(x , y);																		//topLeft
		//move along x coordinate until it reaches end of row

		if (x + (spriteWidth / textureWidth) + border >= 1) {		
			if (height == 0) {
				sSheetWidth = i + 1;
			}
			height++;
			
			//set x back to beginning of spritesheet and move y down to next row
			x = 0;
			y -= ((spriteHeight / textureHeight ) + border);
		}
		else {			
			x += (spriteWidth / textureWidth) + border;
		}
		textureUv.textureId = getTextureId();
		textureUv.spriteSheetId = sprites.size() + 1;
		textureUv.zIndex = zIndex;
		sprites.push_back(textureUv);
		
	}
	sSheetHeight = height;
	
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

geProject::SpriteRender& geProject::SpriteSheet::getSprite(unsigned int atlasId){	
	return sprites[static_cast<std::vector<geProject::SpriteRender, std::allocator<geProject::SpriteRender>>::size_type>(atlasId)];
}

unsigned int geProject::SpriteSheet::getSpriteSize() {
	return sprites.size();
}

unsigned int geProject::SpriteSheet::getSpriteWidth() { return spriteWidth; }
unsigned int geProject::SpriteSheet::getSpriteHeight() { return spriteHeight; }
unsigned int geProject::SpriteSheet::getSpriteSheetHeight() { return sSheetHeight; }
unsigned int geProject::SpriteSheet::getSpriteSheetWidth() { return sSheetWidth; }