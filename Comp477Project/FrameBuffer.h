#pragma once
#include "Texture.h"
class FBODesc
{
public:
	FBODesc();
	int width, height, attachId;
	GLenum format;
	GLenum internalFormat;
};
class FrameBuffer :
	public Texture
{
public:
	FrameBuffer(FBODesc desc);
	//FrameBuffer(vector<FBODesc> colorDescs, FBODesc depthDesc);
	~FrameBuffer();
	void bind();
	void unbind();
	void createColorTarget(TextureDesc &desc);
	void createDepthTarget(TextureDesc &desc);
protected:
	GLuint fboID;
	vector<Texture*> targets;
	Texture* depthTarget;
};

