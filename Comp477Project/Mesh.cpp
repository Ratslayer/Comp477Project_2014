#include "stdafx.h"
#include "Mesh.h"
#include "Window.h"
Mesh::Mesh()
	:Asset()
{
	iVerticesVBO = iBinormalsVBO = iTexCoordsVBO = iNormalsVBO = iIndicesVBO = 0;
	//loadASEFile(fileName);
}
Mesh::~Mesh()
{
	deleteVBO(iVerticesVBO);
	deleteVBO(iNormalsVBO);
	deleteVBO(iTexCoordsVBO);
	deleteVBO(iBinormalsVBO);
	deleteVBO(iIndicesVBO);
}
void Mesh::loadFromFile(std::string fileName)
{
	Asset::loadFromFile(fileName);
	loadASEFile(fileName);
}
void Mesh::loadObjFile(string fileName)
{
	ifstream fileStream(fileName);
	if (fileStream.is_open())
	{
		vector<float> vertices;
		vector<float> texCoords;
		vector<float> rawTexCoords;
		vector<float> normals;
		//vector<vec3> binormals;
		vector<pair<unsigned int, unsigned int>> indexPairs;
		vector<unsigned int> indices;
		string nextLine;
		while (getline(fileStream, nextLine))
		{
			string header;
			istringstream content(nextLine);
			content >> header;
			if (header == "v")
			{
				readVector<float>(content, vertices, 3);
			}
			else if (header == "vt")
			{
				readVector<float>(content, rawTexCoords, 2);
			}
			else if (header == "vn")
			{
				readVector<float>(content, normals, 3);
			}
			else if (header == "f")
			{
				string indexBatch;
				for (int i = 0; i < 3; i++)
				{
					content >> indexBatch;
					istringstream indexStream(indexBatch);
					string firstIndexString, secondIndexString;
					getline(indexStream, firstIndexString, '/');
					getline(indexStream, secondIndexString, '/');
					pair<unsigned int, unsigned int> indexPair;
					indexPair.first = atoi(firstIndexString.c_str());
					indexPair.second = atoi(secondIndexString.c_str());
					indexPairs.push_back(indexPair);
					indices.push_back(indexPair.first-1);
				}
				//readVector<unsigned int>(content, indices, 3);
				/*for (int i = 0; i < 3; i++)
				{
					unsigned int index;
					content >> index;
					indices.push_back(index);
				}*/
			}
		}
		/*for (int i = 0; i < rawTexCoords.size() / 2; i++)
		{
			rawTexCoords[i + 1] = 1 - rawTexCoords[i + 1];
		}*/
		texCoords = rawTexCoords;//getTexCoords(indexPairs, rawTexCoords, vertices.size()/3);
		createVBOs(vertices, texCoords, normals, indices);
		//createVBO<vec3>(binormals, &iBinormalsVBO);
	}

}
void Mesh::bindVBO(GLuint VBOpos, GLuint attribPos, int attribSize)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBOpos);
	glEnableVertexAttribArray(attribPos);
	glVertexAttribPointer(attribPos, attribSize, GL_FLOAT, false, 0, 0);
}
void Mesh::BindVertices(GLuint attribPos)
{
	bindVBO(iVerticesVBO, attribPos, 3);
}
void Mesh::BindTexCoords(GLuint attribPos)
{
	bindVBO(iTexCoordsVBO, attribPos, 2);
}
void Mesh::BindNormals(GLuint attribPos)
{
	bindVBO(iNormalsVBO, attribPos, 3);
}
void Mesh::BindBinormals(GLuint attribPos)
{
	bindVBO(iBinormalsVBO, attribPos, 3);
}
void Mesh::deleteVBO(GLuint iVBO)
{
	if (iVBO)
		glDeleteBuffers(1, &iVBO);
}

void Mesh::draw()
{
	/*BindVertices(effect->getAttribute("position"));
	BindTexCoords(effect->getAttribute("texCoords"));
	BindNormals(effect->getAttribute("normal"));
	BindBinormals(effect->getAttribute("binormal"));*/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iIndicesVBO);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
}

vector<float> Mesh::getTexCoords(vector<pair<unsigned int, unsigned int>> &indexPairs, vector<float> &rawTexCoords, unsigned int numVertices)
{
	//unsigned int numTexCoords = indexPairs.size() / 3;
	vector<float> texCoords;
	for (unsigned int i = 0; i < numVertices; i++)
	{
		vec2 texCoord = getAverageTexCoord(i, indexPairs, rawTexCoords);
		texCoords.push_back(texCoord.x);
		texCoords.push_back(texCoord.y);
	}
	return texCoords;
}

vec2 Mesh::getAverageTexCoord(unsigned int index, vector<pair<unsigned int, unsigned int>> &indexPairs, const vector<float> &rawTexCoords)
{
	vec2 texCoord;
	int numOccurences = 0;
	for (unsigned int i = 0; i < indexPairs.size(); i++)
	{
		unsigned int curIndex = indexPairs[i].first - 1;
		if (curIndex == index)
		{
			unsigned int texCoordIndex = indexPairs[i].second - 1;
			vec2 nextTexCoord = vec2(rawTexCoords[texCoordIndex], rawTexCoords[texCoordIndex + 1]);
			texCoord += nextTexCoord;
			numOccurences++;
		}
	}
	texCoord /= (float)numOccurences;
	return texCoord;
}

void Mesh::loadASEFile(string fileName)
{
	Scanner sc(fileName);
	vector<float> vertices;
	vector<float> texCoords;
	vector<float> normals;
	vector<unsigned int> indices;
	vector<unsigned int> tcIndices;
	//get translation 
	float translation[3];
	sc.jumpAfter("TM_ROW3");
	for (int i = 0; i < 3; i++)
		translation[i] = sc.getFloat();
	//get size of model
	unsigned int numVertices, numFaces;
	sc.jumpAfter("MESH_NUMVERTEX");
	numVertices = sc.getUInt();
	sc.jumpAfter("MESH_NUMFACES");
	numFaces = sc.getUInt();
	//get vertices
	sc.jumpAfter("MESH_VERTEX_LIST");
	for (unsigned int i = 0; i < numVertices; i++)
	{
		sc.jumpAfter("MESH_VERTEX");
		unsigned int id=sc.getUInt();
		for (int j = 0; j < 3; j++)
			vertices.push_back(sc.getFloat() - translation[j]);
	}
	//get faces
	sc.jumpAfter("MESH_FACE_LIST");
	for (unsigned int i = 0; i < numFaces; i++)
	{
		sc.jumpAfter("MESH_FACE");
		sc.jumpAfter(":");
		for (int j = 0; j < 3; j++)
		{
			sc.jumpAfter(":");
			indices.push_back(sc.getUInt());
		}
	}
	//get texture coordinates
	sc.jumpAfter("MESH_NUMTVERTEX");
	unsigned int numTexCoords = sc.getUInt();
	sc.jumpAfter("MESH_TVERTLIST");
	for (unsigned int i = 0; i < numTexCoords; i++)
	{
		sc.jumpAfter("MESH_TVERT");
		sc.getUInt();
		texCoords.push_back(sc.getFloat());
		texCoords.push_back(sc.getFloat());
	}
	sc.jumpAfter("MESH_NUMTVFACES");
	unsigned int numTcIndices = sc.getUInt();
	sc.jumpAfter("MESH_TFACELIST");
	for (unsigned int i = 0; i < numTcIndices; i++)
	{
		sc.jumpAfter("MESH_TFACE");
		sc.getUInt();
		for (int j = 0; j < 3; j++)
			tcIndices.push_back(sc.getUInt());
	}
	unifyIndices(vertices, texCoords, indices, tcIndices);
	createVBOs(vertices, texCoords, normals, indices);
}

void Mesh::createVBOs(vector<float> &vertices, vector<float> &texCoords, vector<float> &normals, vector<unsigned int> &indices)
{
	createVBO<float>(vertices, &iVerticesVBO);
	createVBO<float>(texCoords, &iTexCoordsVBO);
	createVBO<float>(normals, &iNormalsVBO);
	createVBO<unsigned int>(indices, &iIndicesVBO);
	numIndices = indices.size();
}

void Mesh::unifyIndices(vector<float> &vertices, vector<float> &texCoords, vector<unsigned int> &indices, vector<unsigned int> &tcIndices)
{
	vector<float> newVertices, newTexCoords;
	vector<unsigned int> newIndices;
	for (unsigned int i = 0; i < indices.size(); i++)
	{
		for (int j = 0; j < 3; j++)
			newVertices.push_back(vertices[indices[i] * 3 + j]);
		for (int j = 0; j < 2; j++)
			newTexCoords.push_back(texCoords[tcIndices[i] * 2 + j]);
	}
	for (unsigned int i = 0; i < newVertices.size(); i++)
		newIndices.push_back(i);
	vertices = newVertices;
	texCoords = newTexCoords;
	indices = newIndices;
}

