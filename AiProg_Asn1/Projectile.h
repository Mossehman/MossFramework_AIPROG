#pragma once
#include "Entity2D.h"

class Projectile : public Entity2D
{
public:
	glm::vec2 Direction;

	int Damage = 20;
	bool Collided = false;
	bool IsActive = false;

	float MoveSpeed = 600.0f;

	std::vector<Entity2D*> EntitiesToCollideWith;

	virtual void Update(double dt) override;
};

