#pragma once
#include "FrameBuffer.h"
class ColorDepthBuffer :
	public FrameBuffer
{
public:
	ColorDepthBuffer(int height, int width);
	~ColorDepthBuffer();
};

