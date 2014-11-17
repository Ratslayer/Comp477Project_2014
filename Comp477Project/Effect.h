#pragma once
#include "stdafx.h"
//#include "AssetManager.h"
#include "Texture.h"
#include "VertexShader.h"
#include "FragmentShader.h"
class Effect
{
public:
	struct Parameter
	{
		enum ParamType
		{
			PT_UNKNOWN,
			PT_INT,
			PT_FLOAT,
			PT_FLOAT2,
			PT_FLOAT3,
			PT_FLOAT4,
			PT_MAT44,
			PT_TEX2D,
			PT_TEXCUBE
		};
		Parameter(std::string &paramName, GLint program);

		void operator =(mat44 &m);
		void operator =(vec4 &v);
		void operator =(vec3 &v);
		void operator =(vec2 &v);
		void operator =(real &r);
		void operator =(Texture* tex);

		std::string name;
	private:
		ParamType type;
		GLint paramLoc;
		GLint program;
		bool assertType(ParamType type);
		std::string getTypeName(ParamType type);
		void initType();

	};
	Effect(Shader* vertexShader, Shader* fragmentShader);
	Effect(char* vertexName, char* fragmentName);
	~Effect();
	void Bind();
	Parameter& getParam(char* cname);
	bool hasParam(char* cname);
	template<typename T>
	void safeSetParam(char* cname, T &value)
	{
		if (hasParam(cname))
			getParam(cname) = value;
	}
	GLuint getAttribute(char* cname);
	void loadMatrices(mat44 &world, mat44 &view, mat44 &proj);
private: 
	void loadAllParams();
	std::vector<Parameter> params;
	GLuint program = 0;
	Shader* pVertexShader;
	Shader* pFragmentShader;
};

