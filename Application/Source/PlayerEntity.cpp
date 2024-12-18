#include "PlayerEntity.h"
#include "KeyboardController.h"
#include <iostream>

void PlayerEntity::handleKeyPress(double dt)
{
	float speedMultiplier = 1;

	if (CKeyboardController::GetInstance()->IsKeyDown(playerInputs->sprintKey))
	{
		speedMultiplier = sprintMultiplier;
	}

	if (CKeyboardController::GetInstance()->IsKeyDown(playerInputs->leftKey))
	{
		position.x = max((minPos.x - hitboxOffset.x + hitboxHalfSizeX), position.x - moveSpeed * speedMultiplier * dt);
	}
	else if (CKeyboardController::GetInstance()->IsKeyDown(playerInputs->rightKey) && !CKeyboardController::GetInstance()->IsKeyDown(playerInputs->leftKey))
	{
		position.x = min((maxPos.x + hitboxOffset.x - hitboxHalfSizeX), position.x + moveSpeed * speedMultiplier * dt);
	}

	if (CKeyboardController::GetInstance()->IsKeyDown(playerInputs->jumpKey))
	{
		force.y = jumpForce;
	}
}

void PlayerEntity::Update(double dt)
{
	handleKeyPress(dt);

	checkMinMaxPos(); 
	doGravity(gravityValue);
	doPhysics(dt, friction); 
	checkPosUpdate(); 
	doTileCollision(); 
}
