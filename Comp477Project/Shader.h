#pragma once
#include "stdafx.h"
#include "Asset.h"
#include "ShaderFactory.h"
class Shader : public Asset
{
public:
	Shader();
	Shader(Shader &shader);
	~Shader();
	GLuint getShaderID();
	static std::string getShaderLog(GLuint shader);
protected:
	void loadFromFile(std::string fileName, GLenum type);
private:
	GLuint shaderID;
	
};

