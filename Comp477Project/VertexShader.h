#pragma once
#include "Shader.h"
class VertexShader :
	public Shader
{
public:
	VertexShader();
	~VertexShader();
	std::string getClassName(){ return "VertexShader"; }
	void loadFromFile(std::string fileName);
};

