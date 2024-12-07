#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "BaseLevel.h"

struct PathNode {
	glm::vec2 Position;
	std::vector<int> NeighbouringNodesIndex;

	int Passability; // Use an ENUM for this as opposed to actual numerical values

};

struct EntityPathData {
	float localGoal = 0.0f;
	float globalGoal;
	bool hasChecked;
	int ParentIndex;
	PathNode* currNode;

	EntityPathData() {
		ParentIndex = -1;
		currNode = nullptr;
	}
};

class AStar
{
public:
	
	AStar(glm::vec2 tileSize);
	~AStar();

	virtual void Init(int xEnd, int yEnd, int xStart = 0, int yStart = 0);

	std::vector<PathNode*>& GetNodes(void);

	int GetNodeCount(void);


private:
	std::vector<PathNode*> Nodes;
	glm::vec2 tileSize;
	int nodeCount;

};

