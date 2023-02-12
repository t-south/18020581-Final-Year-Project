#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

geProject::Texture::Texture(const char* filePath): filePath(filePath){	
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
	if (data != NULL) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "TEXTURE ERROR: file not read" << std::endl;
	}
	stbi_image_free(data);
}

geProject::Texture::~Texture() {

}

void geProject::Texture::bindTexture() {	
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void geProject::Texture::unbindTexture() {
	glBindTexture(GL_TEXTURE_2D, 0);
}