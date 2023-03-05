#pragma once
#include <string>
#include <glad/glad.h>
#include <iostream>


namespace geProject{
	class Texture {
	public:
		Texture(const char* filePath);
		~Texture();
		void bindTexture();
		void unbindTexture();
		float getWidth();
		float getHeight();
		unsigned int getTextureId();
	protected:
		unsigned int textureId;
	    float textureWidth, textureHeight;
		std::string filePath;
	};
}