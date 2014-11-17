#pragma once
#include "Texture.h"
class FBODesc
{
public:
	FBODesc();
	int width, height;// , attachId;
	//GLenum format;
	//GLenum internalFormat;
};
class FrameBuffer
{
public:
	FrameBuffer(int width, int height);
	//FrameBuffer(vector<FBODesc> colorDescs, FBODesc depthDesc);
	virtual ~FrameBuffer();
	void bind();
	void unbind();
	Texture* getDepth();
	Texture* getColor(int attachId);
	void createColorTarget(TextureDesc &desc);
	void createDepthTarget(TextureDesc &desc);
protected:
	int width, height;
	GLuint fboID;
	vector<Texture*> targets;
	Texture* depthTarget;
};

