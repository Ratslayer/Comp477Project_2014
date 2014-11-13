#pragma once
#include "Shader.h"
class FragmentShader :
	public Shader
{
public:
	FragmentShader();
	~FragmentShader();
	std::string getClassName(){ return "FragmentShader"; }
	void loadFromFile(std::string fileName);
};

