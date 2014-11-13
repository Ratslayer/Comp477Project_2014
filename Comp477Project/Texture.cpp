#include "stdafx.h"
#include "Texture.h"


Texture::Texture()
	:Asset()
{
}
Texture::Texture(TextureDesc &desc)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, desc.internalFormat, desc.width, desc.height, 0, desc.format, GL_UNSIGNED_BYTE, 0);
}

Texture::~Texture()
{
}

void Texture::loadFromFile(std::string fileName)
{
	Asset::loadFromFile(fileName);
	ILuint img_id = ilGenImage();
	ilBindImage(img_id);
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	if (!ilLoadImage(fileName.c_str()))
	{
		ilDeleteImage(img_id);
		std::cout << "Could not load image " << fileName << ". Error #" << glGetError() << std::endl;
	}
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
		ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 
		0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
	//set mipmapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
GLuint Texture::getTextureID()
{
	return textureID;
}
