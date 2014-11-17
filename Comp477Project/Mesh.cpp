#include "stdafx.h"
#include "Mesh.h"
#include "Window.h"
Mesh::Mesh()
	:Asset()
{
	iVerticesVBO = iTangentsVBO = iBitangentsVBO = iTexCoordsVBO = iNormalsVBO = iIndicesVBO = 0;
}
Mesh::~Mesh()
{
	deleteVBO(iVerticesVBO);
	deleteVBO(iTexCoordsVBO);
	deleteVBO(iNormalsVBO);
	deleteVBO(iTangentsVBO);
	deleteVBO(iBitangentsVBO);
	deleteVBO(iIndicesVBO);
}
void Mesh::loadFromFile(std::string fileName)
{
	Asset::loadFromFile(fileName);
	loadASEFile(fileName);
}
/*void Mesh::loadObjFile(string fileName)
{
	ifstream fileStream(fileName);
	if (fileStream.is_open())
	{
		vector<vec3> vertices;
		vector<vec2> texCoords;
		vector<vec2> rawTexCoords;
		vector<vec3> normals;
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
		//	}
		//}
		/*for (int i = 0; i < rawTexCoords.size() / 2; i++)
		{
			rawTexCoords[i + 1] = 1 - rawTexCoords[i + 1];
		}*/
		//texCoords = rawTexCoords;//getTexCoords(indexPairs, rawTexCoords, vertices.size()/3);
		/*createVBOs(vertices, texCoords, normals, indices);
		createVBO<vec3>(binormals, &iBinormalsVBO);
	}

}*/
void Mesh::bindVBO(GLuint VBOpos, GLuint attribPos, int attribSize)
{
	if (attribPos != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBOpos);
		glEnableVertexAttribArray(attribPos);
		glVertexAttribPointer(attribPos, attribSize, GL_FLOAT, false, 0, 0);
	}
}
void Mesh::deleteVBO(GLuint iVBO)
{
	if (iVBO)
		glDeleteBuffers(1, &iVBO);
}

void Mesh::draw(Effect* effect)
{
	bindVBO(iVerticesVBO, effect->getAttribute("position"), 3);
	bindVBO(iTexCoordsVBO, effect->getAttribute("texCoord"), 2);
	bindVBO(iNormalsVBO, effect->getAttribute("normal"), 3);
	bindVBO(iTangentsVBO, effect->getAttribute("tangent"), 3);
	bindVBO(iBitangentsVBO, effect->getAttribute("bitangent"), 3);

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
	vector<vec3> vertices;
	vector<vec2> texCoords;
	vector<vec3> normals, tangents, bitangents;
	vector<unsigned int> indices;
	vector<unsigned int> tcIndices;
	//get translation 
	//float translation[3];
	vec3 translation;
	sc.jumpAfter("TM_ROW3");
	//for (int i = 0; i < 3; i++)
	//	translation[i] = sc.getFloat();
	translation = sc.getVec3();
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
		vec3 vertex = sc.getVec3() - translation;
		vertices.push_back(vertex);
		//for (int j = 0; j < 3; j++)
		//	vertices.push_back(sc.getFloat() - translation[j]);
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
		vec2 tc = sc.getVec2();
		texCoords.push_back(tc);
		//texCoords.push_back(sc.getFloat());
		//texCoords.push_back(sc.getFloat());
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
	generateNormals(vertices, indices, texCoords, normals, tangents, bitangents);
	createVBOs(vertices, texCoords, normals, tangents, bitangents, indices);
}

void Mesh::createVBOs(vector<vec3> &vertices, vector<vec2> &texCoords, vector<vec3> &normals, vector<vec3> &tangents, vector<vec3> &bitangents, vector<unsigned int> &indices)
{
	createVBO<vec3>(vertices, &iVerticesVBO);
	createVBO<vec2>(texCoords, &iTexCoordsVBO);
	createVBO<vec3>(normals, &iNormalsVBO);
	createVBO<vec3>(tangents, &iTangentsVBO);
	createVBO<vec3>(bitangents, &iBitangentsVBO);
	createVBO<unsigned int>(indices, &iIndicesVBO);
	numIndices = indices.size();
}

void Mesh::unifyIndices(vector<vec3> &vertices, vector<vec2> &texCoords, vector<unsigned int> &indices, vector<unsigned int> &tcIndices)
{
	vector<vec3> newVertices;
	vector<vec2> newTexCoords;
	vector<unsigned int> newIndices;
	for (unsigned int i = 0; i < indices.size(); i++)
	{
		/*for (int j = 0; j < 3; j++)
			newVertices.push_back(vertices[indices[i] * 3 + j]);
		for (int j = 0; j < 2; j++)
			newTexCoords.push_back(texCoords[tcIndices[i] * 2 + j]);*/
		newVertices.push_back(vertices[indices[i]]);
		newTexCoords.push_back(texCoords[tcIndices[i]]);
	}
	for (unsigned int i = 0; i < newVertices.size(); i++)
		newIndices.push_back(i);
	vertices = newVertices;
	texCoords = newTexCoords;
	indices = newIndices;
}

void Mesh::generateNormals(vector<vec3> &vertices, vector<unsigned int> &indices, vector<vec2> &texCoords, vector<vec3> &normals, vector<vec3> &tangents, vector<vec3> &bitangents)
{
	normals = tangents = bitangents = vector<vec3>(vertices.size(), vec3(0, 0, 0));
	for (unsigned int i = 0; i < indices.size()-2; i += 3)
	{
		unsigned int id_0 = indices[i], id_1 = indices[i + 1], id_2 = indices[i + 2];
		vec3 edge_1 = vertices[id_1] - vertices[id_0];
		vec3 edge_2 = vertices[id_2] - vertices[id_0];

		vec2 tedge_1 = texCoords[id_0] - texCoords[id_1];
		vec2 tedge_2 = texCoords[id_0] - texCoords[id_2];

		vec3 normal = edge_1.cross(edge_2);
		vec3 tangent((tedge_2.y * edge_1.x - tedge_1.y * edge_2.x),
			(tedge_2.y * edge_1.y - tedge_1.y * edge_2.y),
			(tedge_2.y * edge_1.z - tedge_1.y * edge_2.z));

		vec3 bitangent = tangent.cross(normal);

		normal.normalize();
		tangent.normalize();
		bitangent.normalize();

		normals[id_0] = normal;
		tangents[id_0] = tangent;
		bitangents[id_0] = bitangent;
	}
}

