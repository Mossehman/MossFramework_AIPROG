#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "Vertex.h"
#include <glm/glm.hpp>
#include <vector>

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateQuad(const std::string& meshName, Color color, float length = 1.f);
	static Mesh* animatedMesh(const std::string& meshName, unsigned int row = 0, unsigned int column = 0, float spriteSizeX = 1.f, float spriteSizeY = 1.f);
	static Mesh* compoundedTileMesh(const std::string& meshName, std::vector<glm::vec2> meshCoords);

	static Mesh* GenerateLine(const std::string& meshName, Color color, float length = 1);
	static Mesh* GenerateLineToLine(const std::string& meshName, Color color, glm::vec2 startPos, glm::vec2 endPos);
	static Mesh* GeneratePolygon(const std::string& meshName, Color color, float radius = 1, float nSides = 36);
	static Mesh* GeneratePolygonLine(const std::string& meshName, Color color, float radius = 1, float nSides = 36);

	static Mesh* GenerateCube(const std::string& meshName, Color color, float length = 1.f);
	static Mesh* GenerateRing(const std::string& meshName, Color color, unsigned numSlice, float outerR = 1.f, float innerR = 0.f);
	static Mesh* GenerateSphere(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float radius = 1.f);
	static Mesh* GenerateCone(const std::string& meshName, Color color, unsigned numSlice, float radius, float height);
	static Mesh* GenerateText(const std::string& meshName, unsigned row, unsigned col);
};

#endif