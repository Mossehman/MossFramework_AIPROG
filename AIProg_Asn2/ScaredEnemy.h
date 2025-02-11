#pragma once
#include "Enemy.h"
class ScaredEnemy : public Enemy
{
public:
	virtual glm::vec2 GetDestination() override;
	glm::vec2 GetNearestTileToDestination(glm::vec2 dest);
};

