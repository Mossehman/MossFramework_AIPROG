#pragma once
#include <glm/glm.hpp>
#include <vector>

struct PathNode {
	glm::vec2 Position;
	std::vector<PathNode*> NeighbouringNodes;
	PathNode* ParentNode;
	int Passability; // Use an ENUM for this as opposed to actual numerical values

};

struct EntityPathData {
	float localGoal;
	float globalGoal;
	bool hasChecked;
};

class AStar
{
public:
	
	AStar(glm::vec2 tileSize);

	virtual void Init(int xEnd, int yEnd, int xStart = 0, int yStart = 0);

	std::vector<PathNode*>& GetNodes(void);

	int GetNodeCount(void);


private:
	std::vector<PathNode*> Nodes;
	glm::vec2 tileSize;
	int nodeCount;

};

