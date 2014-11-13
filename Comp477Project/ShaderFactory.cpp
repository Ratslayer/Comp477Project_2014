#include "stdafx.h"
#include "ShaderFactory.h"
#include "Shader.h"
ShaderFactory* ShaderFactory::_factory = NULL;
ShaderFactory::ShaderFactory()
	:shaderFileIdCache()
{
}


ShaderFactory::~ShaderFactory()
{
	for (unsigned int i = 0; i < shaderFileIdCache.size(); i++)
	{
		glDeleteShader(getShaderByIndex(i));
	}
}
void ShaderFactory::init()
{
	if (_factory)
		release();
	_factory = new ShaderFactory();
}
void ShaderFactory::release()
{
	delete _factory;
}
std::string ShaderFactory::getFolderName(GLenum shaderType)
{
	std::string folderName = "";
	if (shaderType == GL_VERTEX_SHADER)
		folderName = "Vertex";
	if (shaderType == GL_FRAGMENT_SHADER)
		folderName = "Fragment";
	folderName += "\\";
	return folderName;
}
std::string ShaderFactory::getFullFileName(std::string fileName, GLenum shaderType)
{
	//char cpath[200];
	std::string fullName = "..\\Comp477Project\\Assets\\Shaders\\";
	//const char* cstr = fullName.c_str();
	//std::string fullPath=_fullpath(cpath, cstr, 200);
	
	fullName += getFolderName(shaderType);
	fullName += fileName;
	fullName += ".txt";
	return fullName;
}
GLuint ShaderFactory::getShaderByIndex(int i)
{
	return shaderFileIdCache[i].second;
}
std::string ShaderFactory::getShaderNameByIndex(int i)
{
	return shaderFileIdCache[i].first;
}
GLuint ShaderFactory::getShader(std::string &name, GLenum shaderType)
{
	std::string fileName = _factory->getFullFileName(name, shaderType);
	GLuint shader = _factory->getShaderFromCache(fileName);
	//if not in cache
	if (shader == 0)
	{
		//create and load the shader
		shader = glCreateShader(shaderType);
		_factory->loadShaderFromFile(shader, fileName);
		//cache newly loaded shader
		std::pair<std::string, GLuint> shaderFileId;
		shaderFileId.first = fileName;
		shaderFileId.second = shader;
		_factory->shaderFileIdCache.push_back(shaderFileId);
	}
	return shader;
}
GLuint ShaderFactory::getShaderFromCache(std::string &fileName)
{
	GLuint shader = 0;
	for (int i = 0; i < shaderFileIdCache.size(); i++)
	{
		if (getShaderNameByIndex(i) == fileName)
		{
			shader = getShaderByIndex(i);
			break;
		}
	}
	return shader;
}
void ShaderFactory::loadShaderFromFile(GLuint shader, std::string &fileName)
{
}
