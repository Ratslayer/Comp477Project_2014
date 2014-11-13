#include "stdafx.h"
#include "FrameBuffer.h"


FrameBuffer::FrameBuffer(FBODesc desc)
{
	glGenFramebuffers(1, &fboID);
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, desc.internalFormat, desc.width, desc.height, 0, desc.format, GL_UNSIGNED_BYTE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + desc.attachId, GL_TEXTURE_2D, textureID, 0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, desc.attachmentPoint, GL_TEXTURE_2D, textureID, 0);
}


FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, textureID);
}

void FrameBuffer::unbind()
{
	glBindTexture(GL_TEXTURE_2D, textureID);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBuffer::createColorTarget(TextureDesc &desc)
{
	Texture* texture = new Texture(desc);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + targets.size(), GL_TEXTURE_2D, textureID, 0);
	targets.push_back(texture);
}

void FrameBuffer::createDepthTarget(TextureDesc &desc)
{
	//desc.format = GL_DEPTH_COMPONENT;
	//desc.internalFormat=GL
	depthTarget = new Texture(desc);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureID, 0);
}

FBODesc::FBODesc()
{
	width = height = 0;
	internalFormat = GL_RGBA8;
	format = GL_RGBA;
	attachId = 0;
}
