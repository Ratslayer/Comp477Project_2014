#include "stdafx.h"
#include "ColorDepthBuffer.h"


ColorDepthBuffer::ColorDepthBuffer(int width, int height)
	:FrameBuffer(width, height)
{
	TextureDesc desc;
	desc.width = width;
	desc.height = height;
	desc.format = GL_RGBA;
	desc.internalFormat = GL_RGBA8;
	createColorTarget(desc);
	createDepthTarget(desc);
}
ColorDepthBuffer::~ColorDepthBuffer()
{
}
