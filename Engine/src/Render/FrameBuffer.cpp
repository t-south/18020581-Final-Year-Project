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
}

geProject::FrameBuffer::~FrameBuffer(){}

void geProject::FrameBuffer::bind(){ glBindFramebuffer(GL_FRAMEBUFFER, fbo); }

void geProject::FrameBuffer::unBind(){ glBindFramebuffer(GL_FRAMEBUFFER, 0); }

unsigned int geProject::FrameBuffer::getTextureId() { return texture->getTextureId(); }
unsigned int geProject::FrameBuffer::getFBOid() { return fbo; }
unsigned int geProject::FrameBuffer::getRBOid() { return rbo; }