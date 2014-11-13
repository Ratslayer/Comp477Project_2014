#include "stdafx.h"
#include "VertexShader.h"


VertexShader::VertexShader()
{
}


VertexShader::~VertexShader()
{
}

void VertexShader::loadFromFile(std::string fileName)
{
	Asset::loadFromFile(fileName);
	Shader::loadFromFile(fileName, GL_VERTEX_SHADER);
}
