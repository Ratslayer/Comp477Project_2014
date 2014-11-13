#pragma once
#include "Asset.h"
#include "freeglut\include\GL\glut.h"
#include "Devil\include\IL\il.h"
#include "Devil\include\IL\ilut.h"
class TextureDesc
{
public:
	TextureDesc();
	int width, height;
	GLenum format;
	GLenum internalFormat;
};
class Texture : public Asset
{
public:
	struct Desc 
	{
		int width;
		int height;

	};
	Texture();
	Texture(TextureDesc &desc);
	~Texture();
	GLuint getTextureID();
	void loadFromFile(std::string fileName);
	std::string getClassName(){ return "Texture"; }
	int unitOffset = 0;
protected:
	GLuint textureID;
};

