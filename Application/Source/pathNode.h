#pragma once
#include <vector>

class pathNode
{
private:
	//check if node is an obstacle or passable
	bool isPassable;
	//have we checked this node during the astar search process?
	bool hasChecked;
	float globalGoal;
	float localGoal;

	float x;
	float y;

	std::vector<pathNode*> neighbourNodes;
	pathNode* parentNode;

public:
	bool getPassable(void);
	void setPassable(bool isPassable);

	bool getCheckedStatus(void);
	void setCheckedStatus(bool hasChecked);

	float getGlobalGoal(void) const;
	void setGlobalGoal(float globalGoal);

	float getLocalGoal(void);
	void setLocalGoal(float localGoal);

	float getX(void);
	void setX(float x);

	float getY(void);
	void setY(float y);

	std::vector<pathNode*>& getNeighbourNodes(void);
	void addToNeighbourNodes(pathNode* nodeToAdd);

	pathNode*& getParentNode(void);
	void setParentNode(pathNode* parentNode);


};

