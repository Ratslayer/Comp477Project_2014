#include "stdafx.h"
#include "Shader.h"
#include "Window.h"
Shader::Shader()
	:Asset()
{	
}

Shader::Shader(Shader &shader)
	:Asset(shader)
{
	shaderID = shader.shaderID;
}

Shader::~Shader()
{
	glDeleteShader(shaderID);
}
std::string Shader::getShaderLog(GLuint shader)
{
	GLint strSize;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &strSize);
	std::string str = "";
	if (strSize > 1)
	{
		char* cstr = new char[strSize];
		glGetShaderInfoLog(shader, strSize, NULL, cstr);
		str += cstr;
		delete[] cstr;
	}
	return str;
}

GLuint Shader::getShaderID()
{
	return shaderID;
}

void Shader::loadFromFile(std::string fileName, GLenum type)
{
	Asset::loadFromFile(fileName);
	shaderID = glCreateShader(type);
	//load file
	std::ifstream fileStream;
	fileStream.open(fileName);
	std::string shaderSource((std::istreambuf_iterator<char>(fileStream)),
		std::istreambuf_iterator<char>());
	//load shader
	const char* cSource = shaderSource.c_str();
	glShaderSource(shaderID, 1, &cSource, NULL);
	glCompileShader(shaderID);
	std::string shaderLog = getShaderLog(shaderID);
	//std::cout << 
	std::cout << "Shader compilation: " << shaderLog << std::endl;
}

/*void Shader::getShaderParams(GLuint shader)
{
	GLint numParams;
}*/

