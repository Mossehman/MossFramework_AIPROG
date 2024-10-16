#pragma once
#include "MeshBuilder.h"
#include "Camera.h"
#include <vector>

class Minimap
{
public:
	virtual void Init();
	virtual void Update(double dt, glm::vec2 playerPos);
	virtual void Render(Camera& camera);
	virtual void Destroy() {}

	void compoundMinimapMesh();
	void recalculateMapScale();

protected:
	glm::vec2 offset;
	glm::vec2 mapOffset;
	glm::vec2 minimapSize;

	bool toUpdateMinimap;
	unsigned int textureID;

	std::vector<glm::vec2> mapMeshData;

	Mesh* minimapMesh;
	Mesh* backgroundMesh;
	Mesh* playerMesh;

	float mapScaleX;
	float mapScaleY;

	glm::vec2 playerPos;
};

