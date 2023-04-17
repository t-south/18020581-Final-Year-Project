#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"





geProject::Texture::Texture(unsigned int width, unsigned int height) {
	filePath = "frameBuffer";
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
}



geProject::Texture::Texture(const char* filePath): filePath(filePath), textureHeight(0), textureWidth(0){	
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
	if (data != NULL) {
		textureWidth = width;
		textureHeight = height;		
		if (nrChannels == 4) {			
			//check if RGBA
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else if (nrChannels == 3) {
			//check if RGB
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
	else {
		std::cout << "TEXTURE ERROR: file not read" << std::endl;
	}
	stbi_image_free(data);
}

geProject::Texture::~Texture() {

}

GLuint geProject::Texture::getTextureId() {
	return textureId;
}

float geProject::Texture::getHeight() {
	return textureHeight;
}

float geProject::Texture::getWidth() {
	return textureWidth;
}

void geProject::Texture::bindTexture() {	
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void geProject::Texture::unbindTexture() {
	glBindTexture(GL_TEXTURE_2D, 0);
}