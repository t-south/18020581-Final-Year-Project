#include "FrameBuffer.h"

geProject::FrameBuffer::FrameBuffer(unsigned int width, unsigned int height){
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	texture = new Texture(width, height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->getTextureId(), 0);
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "FRAME BUFFER NOT COMPLETE" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0,0,width, height);
}

geProject::FrameBuffer::FrameBuffer(unsigned int width, unsigned int height, bool picking) {
	if (picking) {
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glGenTextures(1, &pId);
		glBindTexture(GL_TEXTURE_2D, pId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pId, 0);
		glEnable(GL_TEXTURE_2D);
		unsigned int depth;
		glGenTextures(1, &depth);
		glBindTexture(GL_TEXTURE_2D, depth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);
		glReadBuffer(GL_NONE);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "FRAME BUFFER NOT COMPLETE" << std::endl;
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
	}
}

geProject::FrameBuffer::~FrameBuffer(){}

void geProject::FrameBuffer::bind(){ glBindFramebuffer(GL_FRAMEBUFFER, fbo); }

void geProject::FrameBuffer::unBind(){ glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void geProject::FrameBuffer::bindPicking() { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo); }

void geProject::FrameBuffer::unBindPicking() { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); }

int geProject::FrameBuffer::getPixel(int x, int y) { 
	bind();
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	float pixel[3];

	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &pixel);	

	//std::cout << pixel << std::endl;
	return (int)pixel[0] - 1;


}

unsigned int geProject::FrameBuffer::getTextureId() { return texture->getTextureId(); }
unsigned int geProject::FrameBuffer::getFBOid() { return fbo; }
unsigned int geProject::FrameBuffer::getRBOid() { return rbo; }