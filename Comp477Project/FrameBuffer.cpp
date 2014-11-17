#include "stdafx.h"
#include "FrameBuffer.h"


FrameBuffer::FrameBuffer(int width, int height)
{
	this->width = width;
	this->height = height;
	glGenFramebuffers(1, &fboID);
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);

	/*glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, desc.internalFormat, desc.width, desc.height, 0, desc.format, GL_UNSIGNED_BYTE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + desc.attachId, GL_TEXTURE_2D, textureID, 0);*/
	//glFramebufferTexture2D(GL_FRAMEBUFFER, desc.attachmentPoint, GL_TEXTURE_2D, textureID, 0);
}


FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);
	if (depthTarget)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_TRUE);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::unbind()
{
	for (int i = 0; i < targets.size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, targets[i]->getTextureID());
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void FrameBuffer::createColorTarget(TextureDesc &desc)
{
	Texture* texture = new Texture(desc);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + targets.size(), GL_TEXTURE_2D, texture->getTextureID(), 0);
	targets.push_back(texture);
}

void FrameBuffer::createDepthTarget(TextureDesc &desc)
{
	desc.format = GL_DEPTH_COMPONENT;
	desc.internalFormat = GL_DEPTH_COMPONENT24;
	depthTarget = new Texture(desc);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTarget->getTextureID(), 0);
}

Texture* FrameBuffer::getDepth()
{
	return depthTarget;
}

Texture* FrameBuffer::getColor(int attachId)
{
	Texture* target = NULL;
	if (attachId >= 0 && attachId < targets.size())
		target = targets[attachId];
	return target;
}

FBODesc::FBODesc()
{
	width = height = 0;
	//internalFormat = GL_RGBA8;
	//format = GL_RGBA;
	//attachId = 0;
}
