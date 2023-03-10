#pragma once
#include <string>
#include <glad/glad.h>
#include <iostream>


namespace geProject{
	class Texture {
	public:
		Texture(const char* filePath);
		Texture(unsigned int width, unsigned int height);		
		~Texture();
		void bindTexture();
		void unbindTexture();
		float getWidth();
		float getHeight();
		GLuint getTextureId();
	protected:
		GLuint textureId;
	    float textureWidth, textureHeight;
		std::string filePath;
	};
}