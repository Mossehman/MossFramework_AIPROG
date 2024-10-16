#pragma once
#include "Entity2D.h"
#include "InventoryManager.h"
#include "InputManager.h"

class PlayerEntity : public SingletonTemplate<PlayerEntity>, public Entity2D
{
	friend SingletonTemplate<PlayerEntity>;

public:
	void handleKeyPress(double dt);
	virtual void Update(double dt);

protected:
	float moveSpeed;
	float jumpForce;
	float sprintMultiplier;

	InventoryManager* playerInventory = InventoryManager::GetInstance(); //player inventory is an instance of our inventory manager
	InputManager* playerInputs = InputManager::GetInstance();
};

