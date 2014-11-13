#include "stdafx.h"
#include "FragmentShader.h"


FragmentShader::FragmentShader()
{
}


FragmentShader::~FragmentShader()
{
}

void FragmentShader::loadFromFile(std::string fileName)
{
	Asset::loadFromFile(fileName);
	Shader::loadFromFile(fileName, GL_FRAGMENT_SHADER);
}
