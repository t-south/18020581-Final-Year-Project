#pragma once 
#include <glm/glm.hpp>
#include <iostream>
#include "../ResourceManager/ResourceManager.h"
#include "Texture.h"
namespace geProject {
	class FrameBuffer {
	public:
		FrameBuffer(unsigned int width, unsigned int height);
		FrameBuffer(unsigned int width, unsigned int height, bool picking);
		~FrameBuffer();
		unsigned int getTextureId();
		unsigned int getFBOid();
		unsigned int getRBOid();
		void bind();
		void unBind();
		void bindPicking();
		void unBindPicking();
		int getPixel(int x, int y);
	private:
		unsigned int fbo, rbo, pId;
		Texture* texture{ nullptr };
	};
}