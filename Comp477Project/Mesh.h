#pragma once
#include "Asset.h"
#include "Scanner.h"
#include <sstream>
#include "Effect.h"
using namespace std;
class Mesh : public Asset
{
public:
	Mesh();
	~Mesh();
	/*void BindVertices(GLuint attribPos);
	void BindTexCoords(GLuint attribPos);
	void BindNormals(GLuint attribPos);
	void BindTangents(GLuint attribPos);*/
	
	void draw(Effect* effect);
	void loadFromFile(std::string fileName);
	string getClassName(){ return "Mesh"; }
protected:
	GLenum usageVBO = GL_STATIC_DRAW;
private:
	vec2 getAverageTexCoord(unsigned int index, vector<pair<unsigned int, unsigned int>> &indexPairs, const vector<float> &rawTexCoords);
	vector<float> getTexCoords(vector<pair<unsigned int, unsigned int>> &indexPairs, vector<float> &rawTexCoords, unsigned int numVertices);
	//bind VBOpos to attribPos
	//attribSize is the number of float components in every element of VBO
	void createVBOs(vector<vec3> &vertices, vector<vec2> &texCoords, vector<vec3> &normals, vector<vec3> &tangents, vector<vec3> &bitangents, vector<unsigned int> &indices);
	void bindVBO(GLuint VBOpos, GLuint attribPos, int attribSize);
	void deleteVBO(GLuint iVBO);
	void loadASEFile(string fileName);
	//void loadObjFile(string fileName);
	template <typename T>
	void readVector(istringstream &content, vector<T> &data, int numElements)
	{
		for (int i = 0; i < numElements; i++)
		{
			T element;
			content >> element;
			data.push_back(element);
		}
	}
	template <typename T>
	void createVBO(vector<T> &data, GLuint *iVBO)
	{
		glGenBuffers(1, iVBO);
		glBindBuffer(GL_ARRAY_BUFFER, *iVBO);
		glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(T), data.data(), usageVBO);
	}
	void unifyIndices(vector<vec3> &vertices, vector<vec2> &texCoords, vector<unsigned int> &indices, vector<unsigned int> &tcIndices);
	void generateNormals(vector<vec3> &vertices, vector<unsigned int> &indices, vector<vec2> &texCoords,vector<vec3> &normals, vector<vec3> &tangents, vector<vec3> &bitangents);
	vector<unsigned int> getAllIndices(vector<vec3> &vertices, vec3 vertex);
	void normalizeArray(vector<vec3> &vectors);
	//static void readVector(istringstream &content, vec3 &v);
	//static void readVector(istringstream &content, vec2 &v);
	
	GLuint iVerticesVBO;
	GLuint iTexCoordsVBO;
	GLuint iNormalsVBO;
	GLuint iTangentsVBO;
	GLuint iBitangentsVBO;
	GLuint iIndicesVBO;
	int numIndices = 0;
};

