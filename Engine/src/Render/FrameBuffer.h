#pragma once 
#include <glm/glm.hpp>
#include <iostream>
#include "../ResourceManager/ResourceManager.h"
#include "Texture.h"
namespace geProject {
	class FrameBuffer {
	public:
		FrameBuffer(unsigned int width, unsigned int height);
		~FrameBuffer();
		unsigned int getTextureId();
		unsigned int getFBOid();
		unsigned int getRBOid();
		void bind();
		void unBind();
	private:
		unsigned int fbo, rbo;
		Texture* texture{ nullptr };
	};
}