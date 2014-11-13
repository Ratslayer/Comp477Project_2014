#pragma once
#include "stdafx.h"
#include <fstream>
#include <streambuf>
class ShaderFactory
{
public:
	ShaderFactory();
	~ShaderFactory();
	//static variables
	static void init();
	//static void release();
	static GLuint getShader(std::string &fileName, GLenum shaderType);
private:
	static ShaderFactory* _factory;
	std::vector<std::pair<std::string, GLuint>> shaderFileIdCache;
	void loadShaderFromFile(GLuint shader, std::string&);
	GLuint getShaderFromCache(std::string&);
	GLuint getShaderByIndex(int i);
	std::string getShaderNameByIndex(int i);
	std::string getFullFileName(std::string fileName, GLenum shaderType);
	std::string getFolderName(GLenum shaderType);
};

