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
	private:
		unsigned int textureId;
		std::string filePath;
	};
}