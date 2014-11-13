#include "stdafx.h"
#include "Effect.h"
#include "Window.h"

Effect::Effect(Shader* pVertex, Shader* pFragment)
{
	pVertexShader = pVertex;
	pFragmentShader = pFragment;
	program = glCreateProgram();
	glAttachShader(program, pVertexShader->getShaderID());
	glAttachShader(program, pFragmentShader->getShaderID());
	glLinkProgram(program);
	GLint linkResult;
	MyNamespace::Window::printGlError();
	glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
	if (linkResult == GL_FALSE)
		std::cout << "Error linking program!" << std::endl;
	loadAllParams();
}
void Effect::loadAllParams()
{
	GLint nUniforms = 0;
	glGetProgramInterfaceiv(program, GL_UNIFORM, GL_ACTIVE_RESOURCES, &nUniforms);
	std::vector<GLchar> nameData;
	std::vector<GLenum> properties;
	properties.push_back(GL_NAME_LENGTH);
	properties.push_back(GL_TYPE);
	properties.push_back(GL_ARRAY_SIZE);
	std::vector<GLint> values(properties.size());
	for (int i = 0; i < nUniforms; i++)
	{
		glGetProgramResourceiv(program, GL_UNIFORM, i, properties.size(), &properties[0], values.size(), NULL, &values[0]);

		nameData.resize(values[0]);
		glGetProgramResourceName(program, GL_UNIFORM, i, nameData.size(), NULL, &nameData[0]);
		std::string name((char*)&nameData[0], nameData.size() - 1);

		Parameter param(name, program);
		params.push_back(param);
	}


}

Effect::Parameter& Effect::getParam(char* cname)
{
	std::string paramName = cname;
	Parameter* param = NULL;
	for (unsigned int i = 0; i < params.size(); i++)
	{
		if (params[i].name == paramName)
		{
			param = &params[i];
			break;
		}
	}
	return *param;
}
Effect::Effect(char* vertexName, char* fragmentName)
	:Effect(AssetManager::load<VertexShader>(vertexName),
			AssetManager::load<FragmentShader>(fragmentName))
{

}
void Effect::Bind()
{
	//MyNamespace::Window::printGlError();
	glUseProgram(program);
	//MyNamespace::Window::printGlError();
}
Effect::~Effect()
{
	if (program)
	{
		glDetachShader(program, pVertexShader->getShaderID());
		glDetachShader(program, pFragmentShader->getShaderID());
		glDeleteProgram(program);
	}
}

void Effect::draw(Mesh *mesh)
{
	GLint attrib;
	attrib = glGetAttribLocation(program, "position");
	if (attrib != -1)
		mesh->BindVertices(attrib);
	attrib = glGetAttribLocation(program, "texCoord");
	if (attrib != -1)
		mesh->BindTexCoords(attrib);
	attrib = glGetAttribLocation(program, "normal");
	if (attrib != -1)
		mesh->BindNormals(attrib);
	attrib = glGetAttribLocation(program, "binormal");
	if (attrib != -1)
		mesh->BindBinormals(attrib);
	mesh->draw();
}

void Effect::loadMatrices(mat44 &world, mat44 &view, mat44 &proj)
{
	getParam("mWorld") = world;
	getParam("mView") = view;
	getParam("mProjection") = proj;
}

Effect::Parameter::Parameter(std::string &paramName, GLint program)
{
	name = paramName;
	initType();
	this->program = program;
	paramLoc = glGetUniformLocation(program, name.c_str());
}

void Effect::Parameter::initType()
{
	switch (name[0])
	{
	case 'f':
		switch (name[1])
		{
		case '2':
			type = Parameter::PT_FLOAT2;
			break;
		case '3':
			type = Parameter::PT_FLOAT3;
			break;
		case '4':
			type = Parameter::PT_FLOAT4;
			break;
		default:
			type = Parameter::PT_FLOAT;
			break;
		}
		break;
	case 'm':
		type = Parameter::PT_MAT44;
		break;
	case 't':
		switch (name[1])
		{
		case '2':
			type = Parameter::PT_TEX2D;
			break;
		}
		break;
	default:
		type = Parameter::PT_UNKNOWN;
		break;
	}
}

void Effect::Parameter::operator=(mat44 &m)
{
	if (assertType(PT_MAT44))
		glProgramUniformMatrix4fv(program, paramLoc, 1, false, m.values);
}

void Effect::Parameter::operator=(vec4 &v)
{
	if (assertType(PT_FLOAT4))
		glProgramUniform4f(program, paramLoc, (float)v.x, (float)v.y, (float)v.z, (float)v.w);
}

void Effect::Parameter::operator=(vec3 &v)
{
	if (assertType(PT_FLOAT3))
		glProgramUniform3f(program, paramLoc, (float)v.x, (float)v.y, (float)v.z);
}

void Effect::Parameter::operator=(vec2 &v)
{
	if (assertType(PT_FLOAT2))
		glProgramUniform2f(program, paramLoc, (float)v.x, (float)v.y);
}

void Effect::Parameter::operator=(real &r)
{
	if (assertType(PT_FLOAT))
		glProgramUniform1f(program, paramLoc, (float)r);
}

void Effect::Parameter::operator=(Texture* tex)
{
	if (assertType(PT_TEX2D))
	{
		glUniform1i(paramLoc, tex->unitOffset);
		glActiveTexture(GL_TEXTURE0+tex->unitOffset);
		glBindTexture(GL_TEXTURE_2D, tex->getTextureID());
		//glBindSampler(0, linearFiltering);
		//glProgramUniform1i(tex->getTextureID(), paramLoc, 0);
	}
}

bool Effect::Parameter::assertType(ParamType type)
{
	bool result = this->type == type;
	if (!result)
		std::cout << "Parameter " << name << " is of type " << getTypeName(this->type)
		<< "; Not of type " << getTypeName(type) << "." << std::endl;
	return result;
}

std::string Effect::Parameter::getTypeName(ParamType type)
{
	std::string result;
	switch (type)
	{
	case PT_FLOAT:
		result = "Single";
		break;
	case PT_FLOAT2:
		result = "Vector2";
		break;
	case PT_FLOAT3:
		result = "Vector3";
		break;
	case PT_FLOAT4:
		result = "Vector4";
		break;
	case PT_INT:
		result = "Int";
		break;
	case PT_MAT44:
		result = "Matrix44";
		break;
	case PT_TEX2D:
		result = "Texture2D";
		break;
	case PT_TEXCUBE:
		result = "TextureCUBE";
		break;
	case PT_UNKNOWN:
		result = "UNKNOWN";
		break;
	default:
		result = "ERROR";
		break;
	}
	return result;
}