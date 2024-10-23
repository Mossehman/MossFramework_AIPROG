#pragma once
#include "Entity2D.h"
#include "FiniteState.h"

class EntityAI2D : public Entity2D
{
public:
	void SolveAStar();
	virtual void MoveAlongPath() {}
	
	void InitialiseStates(); //Call this after Init to ensure that all the states are initialised

	void AddState(FiniteState* newState);
	void RemoveState(std::string stateName);

	std::vector<FiniteState*> GetStates(void);
	FiniteState* GetCurrentState(void);
	void SetCurrentState(FiniteState* newCurrentState);

protected:
	std::vector<FiniteState*> finiteStates;
	FiniteState* currentState;

	std::vector<glm::vec2> pathWaypoints; //the list of target positions for the Entity to move towards in sequence
	int currWaypointIndex = 0; //the index of the target waypoint (ensure this resets to 0 everytime the path is re-calculated

	float moveSpeed = 1.0f; //the move speed of the AI entity along it's path

};

