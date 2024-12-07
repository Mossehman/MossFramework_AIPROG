#pragma once
#include "Entity2D.h"
#include "AStar.h"
#include "FiniteStateMachine.h"

class EntityAI2D : public Entity2D
{
public:
	void InitAStar();
	void SolveAStar();
	virtual void MoveAlongPath() {}

	bool RenderPath;

	void RenderNodePath(Color PathColor); 
	glm::vec2 targetPos;

	EntityAI2D(glm::vec2 pos = glm::vec2(0, 0), glm::vec2 rot = glm::vec2(0, 0), glm::vec2 scl = glm::vec2(1, 1));
	FiniteStateMachine* GetFSM();

protected:

	std::vector<glm::vec2> pathWaypoints; //the list of target positions for the Entity to move towards in sequence
	int currWaypointIndex = 0; //the index of the target waypoint (ensure this resets to 0 everytime the path is re-calculated

	std::vector<EntityPathData*> pathData;
	EntityPathData* destinationData;

	float moveSpeed = 1.0f; //the move speed of the AI entity along it's path

	FiniteStateMachine* stateMachine;

};

