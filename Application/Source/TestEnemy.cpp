#include "TestEnemy.h"
#include <random>

TestEnemy::TestEnemy(LevelIDs entityLevel, Entity2D* playerEntity, glm::vec2 pos, glm::vec2 rot, glm::vec2 scl) : playerEntity(nullptr)
{
	nodes = new pathNode[Map2D::GetInstance()->mapSizeX[entityLevel] * Map2D::GetInstance()->mapSizeY[entityLevel]];
	finiteStateMachine = new StateManager();

	this->playerEntity = playerEntity;

	moveSpeed = 100.f;
	jumpForce = 20000.f;

	this->position = pos;
	this->rotation = rot;
	this->scale = scl;

	self_position = glm::vec2(0, 0);
	self_rotation = glm::vec2(0, 0);
	self_scale = glm::vec2(1, 1);

	textureID = 0;

	for (int x = 0; x < Map2D::GetInstance()->mapSizeX[entityLevel]; x++)
	{
		for (int y = 0; y < Map2D::GetInstance()->mapSizeY[entityLevel]; y++)
		{
			int index = y * Map2D::GetInstance()->mapSizeX[entityLevel] + x;

			nodes[index].setX(x * Map2D::GetInstance()->getTileSize().x);
			nodes[index].setY(y * Map2D::GetInstance()->getTileSize().y * -1);
			nodes[index].setPassable(true);
			if (!Map2D::GetInstance()->getMapPassable(y, x, currentLevel) || Map2D::GetInstance()->getMapDangeous(y, x, currentLevel))
			{
				nodes[index].setPassable(false);
			}
			nodes[index].setParentNode(nullptr);
			nodes[index].setCheckedStatus(false);
		}
	}


	for (int x = 0; x < Map2D::GetInstance()->mapSizeX[entityLevel]; x++)
	{
		for (int y = 0; y < Map2D::GetInstance()->mapSizeY[entityLevel]; y++)
		{


			if (y > 0)
			{
				nodes[y * Map2D::GetInstance()->mapSizeX[entityLevel] + x].addToNeighbourNodes(&nodes[(y - 1) * Map2D::GetInstance()->mapSizeX[entityLevel] + x]);
			}

			if (y < Map2D::GetInstance()->mapSizeY[entityLevel] - 1)
			{
				nodes[y * Map2D::GetInstance()->mapSizeX[entityLevel] + x].addToNeighbourNodes(&nodes[(y + 1) * Map2D::GetInstance()->mapSizeX[entityLevel] + x]);
			}


			if (x > 0)
			{
				nodes[y * Map2D::GetInstance()->mapSizeX[entityLevel] + x].addToNeighbourNodes(&nodes[y * Map2D::GetInstance()->mapSizeX[entityLevel] + (x - 1)]);
			}

			if (x < Map2D::GetInstance()->mapSizeX[entityLevel] - 1)
			{
				nodes[y * Map2D::GetInstance()->mapSizeX[entityLevel] + x].addToNeighbourNodes(&nodes[y * Map2D::GetInstance()->mapSizeX[entityLevel] + (x + 1)]);
			}
		}
	}

	startNode = &nodes[(int)Map2D::GetInstance()->posToTilePos(position).y * Map2D::GetInstance()->mapSizeY[entityLevel] + (int)Map2D::GetInstance()->posToTilePos(position + (Map2D::GetInstance()->getTileSize().x * 0.5f)).x];

}

void TestEnemy::initialiseStates()
{
	finiteStateMachine->addState("Idle");
	finiteStateMachine->addState("Patrol");
	finiteStateMachine->addState("AttackMelee");
	finiteStateMachine->addState("Chase");
	finiteStateMachine->addState("Stunned");
	finiteStateMachine->addState("Dead");

	finiteStateMachine->transitionToStateForce("Idle");

}

void TestEnemy::updateFSM(double dt)
{
	//std::cout << "a" << std::endl;

	std::default_random_engine generator;

	std::string currentStateName = finiteStateMachine->currentState->getStateName();

	if (toDestroy)
	{
		finiteStateMachine->transitionToStateForce("Dead");
		return;
	}
	else if (currentStateName == "AttackMelee")
	{
		if (!playerEntity) 
		{ 
			finiteStateMachine->transitionToStateForce("Idle"); 
			return;
		}
		glm::vec2 playerToEnemy = playerEntity->getPosition() - position;
		if ((playerToEnemy.x * playerToEnemy.x) + (playerToEnemy.y * playerToEnemy.y) > attackRange * attackRange) //player is outside attack range
		{
			finiteStateMachine->transitionToStateForce("Chase");
			return;
		}

		attackTimerCurrent += dt;
		if (attackTimerCurrent >= attackTimer)
		{
			playerEntity->setHealth(playerEntity->getHealth() - damageValue);
			attackTimerCurrent = 0;
		}

	}
	else if (currentStateName == "Chase")
	{
		if (!playerEntity)
		{
			finiteStateMachine->transitionToStateForce("Idle");
			return;
		}
		glm::vec2 playerToEnemy = playerEntity->getPosition() - position; 

		if ((playerToEnemy.x * playerToEnemy.x) + (playerToEnemy.y * playerToEnemy.y) > viewRange * viewRange) //player is outside view range
		{
			finiteStateMachine->transitionToStateForce("Patrol");
			return;
		}
		else if ((playerToEnemy.x * playerToEnemy.x) + (playerToEnemy.y * playerToEnemy.y) <= attackRange * 0.8f * attackRange * 0.8f) //player is outside attack range
		{
			finiteStateMachine->transitionToStateForce("AttackMelee");
			attackTimerCurrent = 0;
			return;
		}

		targetPosition = playerEntity->getPosition();
		solveAStar();
		getCurrentPath();
		pathFind(dt);
	}

	else if (currentStateName == "Patrol")
	{
		randomPatrolTimerCurrent += dt;

		if (playerEntity)
		{
			glm::vec2 playerToEnemy = playerEntity->getPosition() - position;

			if ((playerToEnemy.x * playerToEnemy.x) + (playerToEnemy.y * playerToEnemy.y) <= viewRange * viewRange) //player is within view range
			{
				finiteStateMachine->transitionToStateForce("Chase");
				return;
			}
		}
		
		position.x += moveSpeed * dt * xPatrolDirection;

		if (!Map2D::GetInstance()->getMapPassable(Map2D::GetInstance()->posToTilePos(position).y, Map2D::GetInstance()->posToTilePos(position).x + xPatrolDirection, currentLevel) ||
			!Map2D::GetInstance()->getMapPassable(Map2D::GetInstance()->posToTilePos(position).y + 1, Map2D::GetInstance()->posToTilePos(position).x + xPatrolDirection, currentLevel))
		{
			xPatrolDirection *= -1;
		}

		if (randomPatrolTimerCurrent >= randomPatrolTimer)
		{
			std::uniform_int_distribution<int> distribution(1, 2);
			int toSwapState = distribution(generator);

			if (toSwapState == 1)
			{
				finiteStateMachine->transitionToStateForce("Idle");
			}
			randomPatrolTimerCurrent = 0;
		}
	}
	else if (currentStateName == "Idle")
	{
		randomPatrolTimerCurrent += dt;

		if (playerEntity) 
		{
			glm::vec2 playerToEnemy = playerEntity->getPosition() - position; 

			if ((playerToEnemy.x * playerToEnemy.x) + (playerToEnemy.y * playerToEnemy.y) <= viewRange * viewRange) //player is within view range 
			{
				finiteStateMachine->transitionToStateForce("Chase"); 
				return;
			}
		}

		if (randomPatrolTimerCurrent >= randomPatrolTimer) 
		{
			std::uniform_int_distribution<int> distribution(1, 2); 
			int toSwapState = distribution(generator); 

			if (toSwapState == 1) 
			{
				finiteStateMachine->transitionToStateForce("Patrol"); 
			}
			randomPatrolTimerCurrent = 0; 

		}

	}
}
