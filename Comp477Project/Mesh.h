#pragma once
#include "Asset.h"
#include "Scanner.h"
#include <sstream>
using namespace std;
class Mesh : public Asset
{
public:
	void BindVertices(GLuint attribPos);
	void BindTexCoords(GLuint attribPos);
	void BindNormals(GLuint attribPos);
	void BindBinormals(GLuint attribPos);
	Mesh();
	~Mesh();
	void draw();
	void loadFromFile(std::string fileName);
	string getClassName(){ return "Mesh"; }
protected:
	GLenum usageVBO = GL_STATIC_DRAW;
private:
	vec2 getAverageTexCoord(unsigned int index, vector<pair<unsigned int, unsigned int>> &indexPairs, const vector<float> &rawTexCoords);
	vector<float> getTexCoords(vector<pair<unsigned int, unsigned int>> &indexPairs, vector<float> &rawTexCoords, unsigned int numVertices);
	//bind VBOpos to attribPos
	//attribSize is the number of float components in every element of VBO
	void createVBOs(vector<float> &vertices, vector<float> &texCoords, vector<float> &normals, vector<unsigned int> &indices);
	void bindVBO(GLuint VBOpos, GLuint attribPos, int attribSize);
	void loadASEFile(string fileName);
	void loadObjFile(string fileName);
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
	void unifyIndices(vector<float> &vertices, vector<float> &texCoords, vector<unsigned int> &indices, vector<unsigned int> &tcIndices);
	//static void readVector(istringstream &content, vec3 &v);
	//static void readVector(istringstream &content, vec2 &v);
	void deleteVBO(GLuint iVBO);
	GLuint iVerticesVBO;
	GLuint iTexCoordsVBO;
	GLuint iNormalsVBO;
	GLuint iBinormalsVBO;
	GLuint iIndicesVBO;
	int numIndices = 0;
};

