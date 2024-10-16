#include "FlyingEnemy.h"
#include <random>

FlyingEnemy::FlyingEnemy(LevelIDs entityLevel, Entity2D* playerEntity, glm::vec2 pos, glm::vec2 rot, glm::vec2 scl): playerEntity(nullptr)
{
	{
		nodes = new pathNode[Map2D::GetInstance()->mapSizeX[entityLevel] * Map2D::GetInstance()->mapSizeY[entityLevel]];
		finiteStateMachine = new StateManager();

		this->playerEntity = playerEntity;

		moveSpeed = 150.f;
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
}

void FlyingEnemy::initialiseStates()
{
	finiteStateMachine->addState("Idle");
	finiteStateMachine->addState("Patrol");
	finiteStateMachine->addState("Chase");
	finiteStateMachine->addState("Dead");
	finiteStateMachine->addState("Melee Attack");
	finiteStateMachine->addState("Melee Damage");
	finiteStateMachine->addState("Ranged Attack");
	finiteStateMachine->addState("Flee");

	rangedAttack = new GunBase();
	rangedAttack->setBulletCount(100000);
	rangedAttack->setBulletCountMax(100000);
	rangedAttack->setBurst(true);
	rangedAttack->setBurstInterval(4);
	rangedAttack->setBurstCount(8);
	rangedAttack->setMagCount(100);
	rangedAttack->setMagCountMax(100);
	rangedAttack->setDamage(5);
	playerVector.push_back(playerEntity);
}

void FlyingEnemy::updateFSM(double dt)
{
	std::default_random_engine generator;



	rangedAttack->Update(dt, playerEntity->getPosition(), playerVector);



	std::string currentStateName = finiteStateMachine->currentState->getStateName();

	if (toDestroy)
	{
		finiteStateMachine->transitionToStateForce("Dead");
		return;
	}
	else if (!playerEntity)
	{
		finiteStateMachine->transitionToStateForce("Idle");
		return;
	}
	else if (health <= 10)
	{
		finiteStateMachine->transitionToStateForce("Patrol");
		moveSpeed *= 1.3;
		return;
	}
	else if (currentStateName == "Melee Damage")
	{
		glm::vec2 playerToEnemy = playerEntity->getPosition() - position;
		if ((playerToEnemy.x * playerToEnemy.x) + (playerToEnemy.y * playerToEnemy.y) > attackRange * attackRange) //player is outside attack range
		{
			finiteStateMachine->transitionToStateForce("Melee Attack");
			meleeAttackTimerCurrent = 0;
			return;
		}

		meleeAttackTimerCurrent += dt;
		if (meleeAttackTimerCurrent > meleeAttackTimer)
		{
			playerEntity->setHealth(playerEntity->getHealth() - this->damageValue);
			meleeAttackTimerCurrent = 0;
		}
	}
	else if (currentStateName == "Melee Attack")
	{
		glm::vec2 playerToEnemy = playerEntity->getPosition() - position;
		if ((playerToEnemy.x * playerToEnemy.x) + (playerToEnemy.y * playerToEnemy.y) > meleeAttackDist * meleeAttackDist)
		{
			finiteStateMachine->transitionToStateForce("Ranged Attack");
			return;
		}
		else if ((playerToEnemy.x * playerToEnemy.x) + (playerToEnemy.y * playerToEnemy.y) <= attackRange * 0.7 * attackRange * 0.7) //player is within attack range
		{
			finiteStateMachine->transitionToStateForce("Melee Damage");
			return;
		}

		targetPosition = playerEntity->getPosition();
		solveAStar();
		getCurrentPath();
		pathFind(dt);
	}
	else if (currentStateName == "Ranged Attack")
	{
		glm::vec2 playerToEnemy = playerEntity->getPosition() - position;
		if ((playerToEnemy.x * playerToEnemy.x) + (playerToEnemy.y * playerToEnemy.y) > rangedAttackDist * rangedAttackDist)
		{
			finiteStateMachine->transitionToStateForce("Chase");
			return;
		}
		else if ((playerToEnemy.x * playerToEnemy.x) + (playerToEnemy.y * playerToEnemy.y) <= meleeAttackDist * 0.7 * meleeAttackDist * 0.7) //player is within attack range
		{
			finiteStateMachine->transitionToStateForce("Melee Attack");
			return;
		}

		rangedAttack->shootPos = position; 
		rangedAttack->shootDir = glm::normalize(playerEntity->getPosition() - this->position);
		rangedAttack->Shoot();
		
	}
	else if (currentStateName == "Chase")
	{
		glm::vec2 playerToEnemy = playerEntity->getPosition() - position; 

		if ((playerToEnemy.x * playerToEnemy.x) + (playerToEnemy.y * playerToEnemy.y) > viewRange * viewRange) //player is outside view range
		{
			finiteStateMachine->transitionToStateForce("Patrol");
			return;
		}
		else if ((playerToEnemy.x * playerToEnemy.x) + (playerToEnemy.y * playerToEnemy.y) <= rangedAttackDist * 0.8f * rangedAttackDist * 0.8f) //player is outside attack range
		{
			finiteStateMachine->transitionToStateForce("Ranged Attack");
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

		if (!Map2D::GetInstance()->getMapPassable(Map2D::GetInstance()->posToTilePos(position).y, Map2D::GetInstance()->posToTilePos(position).x + xPatrolDirection, currentLevel))
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

void FlyingEnemy::Render()
{
	if (!toRender || currentLevel != Map2D::GetInstance()->getCurrentLevel()) { return; }

	RenderParameters::GetInstance()->modelStack.PushMatrix();
	RenderParameters::GetInstance()->modelStack.Translate(position.x, position.y, 0);
	RenderParameters::GetInstance()->modelStack.Rotate(rotation.x, 1, 0, 0);
	RenderParameters::GetInstance()->modelStack.Rotate(rotation.y, 0, 1, 0);
	RenderParameters::GetInstance()->modelStack.Scale(scale.x, scale.y, 1);

	if (!child.empty())
	{
		for (int i = 0; i < child.size(); i++)
		{
			child[i]->Render();
		}
	}

	//std::cout << position.x << "," << position.y << std::endl;

	RenderParameters::GetInstance()->modelStack.PushMatrix();
	RenderParameters::GetInstance()->modelStack.Translate(self_position.x, self_position.y, 0);
	RenderParameters::GetInstance()->modelStack.Rotate(self_rotation.x, 1, 0, 0);
	RenderParameters::GetInstance()->modelStack.Rotate(self_rotation.y, 0, 1, 0);
	RenderParameters::GetInstance()->modelStack.Scale(self_scale.x, self_scale.y, 1);



	renderMesh();

	RenderParameters::GetInstance()->modelStack.PopMatrix();
	RenderParameters::GetInstance()->modelStack.PopMatrix();

	for (int i = 0; i < rangedAttack->bullets.size(); i++)
	{
		if (rangedAttack->bullets[i] == nullptr) { continue; }
		rangedAttack->bullets[i]->Render();
	}
}
