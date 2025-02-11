#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

class TurnBasedEntity
{
public:

	bool isTurn = false;
	int movementRange = 3;
	std::vector<glm::vec2> path;
	virtual void OnMovementStart(glm::vec2 destination) = 0;
	virtual void OnTurnStart() = 0;
	virtual void OnTurnEnd() = 0;
};

