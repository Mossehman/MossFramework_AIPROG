#define NOMINMAX
#include "Entity2D.h"
#include "KeyboardController.h"
#include "Map2D.h"
#include <limits>
#include "MeshBuilder.h"

//iteration count for our tilemap collision checks before it defaults to the max world border
#define MAX_CHECK_COUNT_X 10
#define MAX_CHECK_COUNT_Y 10

Entity2D::Entity2D(glm::vec2 pos, glm::vec2 rot, glm::vec2 scl)
{
	position = pos;
	rotation = rot;
	scale = scl;

	health = 100;
	toDestroy = false;

	self_position = glm::vec2(0, 0);
	self_rotation = glm::vec2(0, 0);
	self_scale = glm::vec2(1, 1);

	textureID = 0;
}

void Entity2D::Init(int currentLevel, bool isGravity, bool isPhysics, bool checkTileCollisions, bool entityCollision)
{
	//defines the current level the entity resides on
	this->currentLevel = currentLevel;

	this->isGravity = isGravity;
	this->isPhysics = isPhysics;
	this->checkTileCollisions = checkTileCollisions;
	this->entityCollision = entityCollision;

	//cache the min and max pos to avoid recalculating the values with each frame
	this->cachedMaxPos.x = (Map2D::GetInstance()->TileToPos(glm::vec2(Map2D::GetInstance()->mapSizeX[currentLevel] - 1, 0)) + (Map2D::GetInstance()->GetTileSize() * 0.5f)).x;
	this->cachedMinPos.x = (Map2D::GetInstance()->TileToPos(glm::vec2(0, Map2D::GetInstance()->mapSizeY[currentLevel])) - (Map2D::GetInstance()->GetTileSize() * 0.5f)).x;

	this->cachedMaxPos.y = (Map2D::GetInstance()->TileToPos(glm::vec2(Map2D::GetInstance()->mapSizeX[currentLevel], 0)) + (Map2D::GetInstance()->GetTileSize() * 0.5f)).y;
	this->cachedMinPos.y = (Map2D::GetInstance()->TileToPos(glm::vec2(0, Map2D::GetInstance()->mapSizeY[currentLevel] - 1)) - (Map2D::GetInstance()->GetTileSize() * 0.5f)).y;


	this->minPos = cachedMinPos;
	this->maxPos = cachedMaxPos;
}

void Entity2D::Update(double dt)
{
	if (currentLevel != Map2D::GetInstance()->GetCurrentLevel() && !updateInBackend) { return; }
	if (this->health <= 0)
	{
		toDestroy = true;
	}

	checkMinMaxPos();
	doGravity(gravityValue);
	doPhysics(dt, friction);
	checkPosUpdate();
	doTileCollision();

}

void Entity2D::checkMinMaxPos()
{
	if (!checkTileCollisions) { return; }

	if (xUpdate) { checkBorderY(); }
	if (yUpdate) { checkBorderX(); }

	xUpdate = false;
	yUpdate = false;

}

void Entity2D::checkBorderX()
{
	glm::vec2 hitboxTile = Map2D::GetInstance()->PosToTilePos(position + hitboxOffset); //get the tile coordinates of the hitbox
	resetBorderX(); //reset our X coordinate borders when the y value updates

	//std::cout << hitboxTile.x << hitboxTile.y << std::endl;

	/*std::cout << Map2D::GetInstance()->getMapInfo(hitboxTile.x, hitboxTile.y, Map2D::GetInstance()->getCurrentLevel()) << std::endl;*/

	//we want to check in the left and right dir, so we need to get the y coordinates of the hitbox to determine the hitbox height in tiles (rounded up)

	int minHitboxY = Map2D::GetInstance()->PosToTilePos(position + hitboxOffset + hitboxSize * 0.5f - (Map2D::GetInstance()->GetTileSize() * 0.5f) - 1.f, 0).y; //get the minimum hitbox Y value and check the tile it is on
	int maxHitboxY = Map2D::GetInstance()->PosToTilePos(position + hitboxOffset - hitboxSize * 0.5f - (Map2D::GetInstance()->GetTileSize() * 0.5f) + 1.f, 0).y; //get the maximum hitbox Y value and check the tile it is on

	/*
		X - hitboxPos
		A - hitboxPos + hitboxHeight
		B - hitboxPos - hitboxHeight
		                               |///////|
		 +---A---+ ---- maxTileY ----->|///////|
		 |       |                     +-------+
		 |	 X   |                     |///////|
		 |		 |                     |///////|
		 +---B---+ ---- minTileY ----->|///////|

		 - in the event the hitbox overlaps multiple tiles, we will check the maximum and minimum Y values of the hitbox and convert them into tile coordinates
		 - since the tile coordinates are rounded up, we will essentially be telling the code that the hitbox completely occupies both tiles
		 - after that, check these 2 rows and compare the lowest value in +ve X for the maximum X (and the highest value in -ve X for the minimum X)
	
	*/
	

	//std::cout << minHitboxY << ", " << maxHitboxY << ", --> " << hitboxTile.y << std::endl;

	//we need to check left and right for the number of tile rows the hitbox height takes up
	for (int y = minHitboxY; y <= maxHitboxY; y++)
	{
		//check the current row of tiles that i is on, stop when either a tile is hit or we hit the max iteration count

		//check right
		for (int x = hitboxTile.x + 1; x < Map2D::GetInstance()->mapSizeX[Map2D::GetInstance()->GetCurrentLevel()]; x++)
		{
			//std::cout << Map2D::GetInstance()->getMapInfo(x, y, Map2D::GetInstance()->getCurrentLevel()) << std::endl;
			//TODO: change this to check if the tile is solid rather than just air
			if (!Map2D::GetInstance()->GetMapPassable(y, x, Map2D::GetInstance()->GetCurrentLevel()))
			{
				std::cout << "Tile ID of impassable tile: " << Map2D::GetInstance()->GetMapInfo(y, x, Map2D::GetInstance()->GetCurrentLevel()) << std::endl;

				//get the x coordinates of the far left tile border
				float tileXmax = Map2D::GetInstance()->TileToPos(glm::vec2(x, y)).x - Map2D::GetInstance()->GetTileSize().x * 0.5f;
				
				//if the calculated max X is smaller than the current max X, set that as the current max X
				if (tileXmax < maxPos.x) { maxPos.x = tileXmax; }

				std::cout << "Tile Max X: " << maxPos.x << std::endl;
				
				//end the check on that row, check the other rows
				break;
			}
		}

		//check left
		for (int x = hitboxTile.x; x >= 0; x--)
		{
			//TODO: change this to check if the tile is solid rather than just air
			if (!Map2D::GetInstance()->GetMapPassable(y, x, Map2D::GetInstance()->GetCurrentLevel()))
			{
				//get the x coordinates of the far right tile border
				float tileXmin = Map2D::GetInstance()->TileToPos(glm::vec2(x, y)).x + Map2D::GetInstance()->GetTileSize().x * 0.5f;

				//if the calculated min x is larger than the current min x, set that as our current min X
				if (tileXmin > minPos.x) { minPos.x = tileXmin; }

				std::cout << "Tile Min X: " << minPos.x << std::endl;

				//end the check on that row, check the other rows
				break;
			}
		}
	}


}

void Entity2D::checkBorderY()
{
	glm::vec2 hitboxTile = Map2D::GetInstance()->PosToTilePos(position + hitboxOffset, 0); // get the tile coordinates of the hitbox
	resetBorderY(); // reset our X coordinate borders when the y value updates

	// Calculate the tile positions where the hitbox edges lie
	int minHitboxX = Map2D::GetInstance()->PosToTilePos(position + hitboxOffset - hitboxSize * 0.5f + Map2D::GetInstance()->GetTileSize() * 0.5f + 1.f, 0).x; // floor
	int maxHitboxX = Map2D::GetInstance()->PosToTilePos(position + hitboxOffset + hitboxSize * 0.5f + Map2D::GetInstance()->GetTileSize() * 0.5f - 1.f, 0).x;  // ceil

	// Iterate over the columns the hitbox overlaps
	for (int x = minHitboxX; x <= maxHitboxX; x++)
	{
		// Check the current column of tiles that the hitbox is on, stop when either a tile is hit or we hit the max iteration count
		// Check up
		for (int y = hitboxTile.y; y >= 0; y--)
		{
			if (!Map2D::GetInstance()->GetMapPassable(y, x, Map2D::GetInstance()->GetCurrentLevel()))
			{
				// Get the y coordinates of the bottom side of the tile
				float tileYmax = Map2D::GetInstance()->TileToPos(glm::vec2(x, y)).y - Map2D::GetInstance()->GetTileSize().y * 0.5f;

				// If the calculated max Y is smaller than the current max Y, set that as the current max Y
				if (tileYmax < maxPos.y) { maxPos.y = tileYmax; }
				break;
			}
		}

		// Check down
		for (int y = hitboxTile.y + 1; y < Map2D::GetInstance()->mapSizeY[Map2D::GetInstance()->GetCurrentLevel()]; y++)
		{
			if (!Map2D::GetInstance()->GetMapPassable(y, x, Map2D::GetInstance()->GetCurrentLevel()))
			{
				// Get the y coordinates of the top side of the tile
				float tileYmin = Map2D::GetInstance()->TileToPos(glm::vec2(x, y)).y + Map2D::GetInstance()->GetTileSize().y * 0.5f;

				// If the calculated min Y is larger than the current min Y, set that as the current min Y
				if (tileYmin > minPos.y) { minPos.y = tileYmin; }
				break;
			}
		}
	}
}

void Entity2D::resetBorderX()
{
	//maximum y value will be 0 (because the tiles start at y 0 and are rendered downwards)
	//maximum x value will be the map size

	//when tileToPos is calculated, the inital position will be the center of the tile, so we need to reduce this value by the tile's dimensions (refer to diagram)
	maxPos.x = cachedMinPos.x;
	minPos.x = cachedMaxPos.x;

	//std::cout << minPos.x << std::endl;

	/*
	^	XX -> position that tileToPos will return (center of tile) 
	| 
	| 
	|						 direction to check for minY 
	|									  | 
	|								  +-------+
	|								  |       |   
	|  direction to check for maxX -> |	  X   | <- direction to check for minX
	|								  |		  |
	|								  +-------+	
	|									   |
	|						  direction to check for maxY
	|
	|																
  Y +---------------------------------------------------------->
	X
	
	*/
}

void Entity2D::resetBorderY()
{
	maxPos.y = cachedMinPos.y;
	minPos.y = cachedMaxPos.y;
}

void Entity2D::doTileCollision()
{
	while (true)
	{

		float resolutionX = std::numeric_limits<float>::max(), resolutionY = std::numeric_limits<float>::max();
		float resolutionXmin = std::numeric_limits<float>::max(), resolutionXmax = std::numeric_limits<float>::max();
		float resolutionYmin = std::numeric_limits<float>::max(), resolutionYmax = std::numeric_limits<float>::max();

		bool isCollidingX = false, isCollidingY = false;

		// Check for X-axis collisions
		if (position.x + hitboxOffset.x - hitboxSize.x * 0.5f < minPos.x)
		{
			resolutionXmin = minPos.x - (position.x + hitboxOffset.x - hitboxHalfSizeX);
			isCollidingX = true;
		}
		if (position.x + hitboxOffset.x + hitboxSize.x * 0.5f > maxPos.x)
		{
			resolutionXmax = maxPos.x - (position.x - hitboxOffset.x + hitboxHalfSizeX);
			isCollidingX = true;
		}

		if (isCollidingX)
		{
			resolutionX = (std::abs(resolutionXmin) < std::abs(resolutionXmax)) ? resolutionXmin : resolutionXmax;
		}

		// Check for Y-axis collisions
		if (position.y + hitboxOffset.y - hitboxSize.y * 0.5f < minPos.y)
		{
			resolutionYmin = minPos.y - (position.y + hitboxOffset.y - (hitboxHalfSizeY));
			isCollidingY = true;
		}
		if (position.y + hitboxOffset.y + hitboxSize.y * 0.5f > maxPos.y)
		{
			resolutionYmax = maxPos.y - (position.y - hitboxOffset.y + (hitboxHalfSizeY));
			isCollidingY = true;
		}

		if (isCollidingY)
		{
			resolutionY = (std::abs(resolutionYmin) < std::abs(resolutionYmax)) ? resolutionYmin : resolutionYmax;
		}

		if (!isCollidingY && !isCollidingX)
		{
			break;
		}

		// Resolve collision by smallest penetration first
		if (isCollidingX || isCollidingY)
		{
			if ((isCollidingX && (std::abs(resolutionX) < std::abs(resolutionY)) || !isCollidingY))
			{
				position.x += resolutionX;


				if (resolutionX < 0 && velocity.x > 0)
				{
					velocity.x = 0;
				}
				else if (resolutionX > 0 && velocity.x < 0)
				{
					velocity.x = 0;
				}
			}
			else if (isCollidingY)
			{
				position.y += resolutionY;

				if (resolutionY < 0 && velocity.y > 0)
				{
					velocity.y = 0;
				}
				else if (resolutionY > 0 && velocity.y < 0)
				{
					velocity.y = 0;
					isGrounded = true;
				}

				break;
			}

		}

		// Update collision checks to see if we are still colliding after resolution
	}


	//std::cout << maxPos.x << ", " << minPos.y << std::endl;

	//position.x = std::max(minPos.x + hitboxOffset.x + hitboxHalfSizeX, position.x);
	//position.y = std::max(minPos.y + hitboxOffset.y + hitboxHalfSizeY, position.y);

	//position.x = std::min(maxPos.x - hitboxOffset.x - hitboxHalfSizeX, position.x);
	//position.y = std::min(maxPos.y - hitboxOffset.y - hitboxHalfSizeY, position.y);
}


void Entity2D::doPhysics(double dt, float airResistance, float timeScale)
{
	if (!isPhysics) { return; }

	float time = timeScale * dt;
	glm::vec2 friction = velocity * -airResistance;
	friction.y = 0;
	force += friction;

	acceleration.y = force.y;
	acceleration.x = force.x / mass;

	iVelocity = velocity;
	velocity += acceleration * time;

	position += (velocity * time) - (0.5f * acceleration * time * time);

	force = glm::vec2(0, 0);
	//checkPosUpdate();
}

void Entity2D::doGravity(glm::vec2 gravity)
{
	if (isGravity)
	{
		force += mass * gravity * gravityScale;
		yUpdate = true;
	}
}

void Entity2D::debugMove(float speed, double dt)
{
	if (CKeyboardController::GetInstance()->IsKeyDown('W'))
	{
		force.y = 1 * speed;
	}
	if (CKeyboardController::GetInstance()->IsKeyDown('A'))
	{
		velocity.x = -0.1 * speed;
	}
	if (CKeyboardController::GetInstance()->IsKeyDown('D'))
	{
		velocity.x = 0.1 * speed;
	}

}

void Entity2D::checkPosUpdate()
{
	if (originalPos.x != position.x)
	{
		xUpdate = true;
		originalPos.x = position.x;
	}
	if (originalPos.y != position.y)
	{
		yUpdate = true;
		originalPos.y = position.y;
	}

}

bool Entity2D::checkEntityCollision(Entity2D* entityToCheck)
{
	if (!entityToCheck || entityToCheck == this || !entityCollision) { return false; }

	glm::vec2 AABBminA, AABBmaxA, AABBminB, AABBmaxB;

	//calculate hitbox bounds
	AABBminA = this->position + this->hitboxOffset - (this->hitboxSize * 0.5f);
	AABBmaxA = this->position + this->hitboxOffset + (this->hitboxSize * 0.5f);

	AABBminB = entityToCheck->getPosition() + entityToCheck->getHitboxOffset() - (entityToCheck->getHitboxSize() * 0.5f);
	AABBmaxB = entityToCheck->getPosition() + entityToCheck->getHitboxOffset() + (entityToCheck->getHitboxSize() * 0.5f);

	// check for collision via AABB
	bool collisionX = AABBminA.x <= AABBmaxB.x && AABBmaxA.x >= AABBminB.x;
	bool collisionY = AABBminA.y <= AABBmaxB.y && AABBmaxA.y >= AABBminB.y;

	// If colliding on both axes, returns true, else returns false
	return collisionX && collisionY;
}

void Entity2D::resolveEntityCollision(Entity2D* entityToCheck)
{
	if (!entityToCheck || entityToCheck == this || !entityCollision) { return; }

	glm::vec2 AABBminA = this->position + this->hitboxOffset - (this->hitboxSize * 0.5f);
	glm::vec2 AABBmaxA = this->position + this->hitboxOffset + (this->hitboxSize * 0.5f);

	glm::vec2 AABBminB = entityToCheck->getPosition() + entityToCheck->getHitboxOffset() - (entityToCheck->getHitboxSize() * 0.5f);
	glm::vec2 AABBmaxB = entityToCheck->getPosition() + entityToCheck->getHitboxOffset() + (entityToCheck->getHitboxSize() * 0.5f);
	
	float overlapX = 0.0f;
	if (AABBmaxA.x > AABBminB.x && AABBminA.x < AABBmaxB.x)
	{
		float overlapLeft = AABBmaxA.x - AABBminB.x; 
		float overlapRight = AABBmaxB.x - AABBminA.x;
		overlapX = (overlapLeft < overlapRight) ? -overlapLeft : overlapRight;
	}

	// Calculate overlap on the y-axis
	float overlapY = 0.0f;
	if (AABBmaxA.y > AABBminB.y && AABBminA.y < AABBmaxB.y)
	{
		float overlapBottom = AABBmaxA.y - AABBminB.y;
		float overlapTop = AABBmaxB.y - AABBminA.y;
		overlapY = (overlapBottom < overlapTop) ? -overlapBottom : overlapTop;
	}

	if (std::abs(overlapX) < std::abs(overlapY))
	{
		this->position.x += overlapX;
		if (overlapX > 0 || overlapX < 0)
		{
			this->velocity.x = 0;
		}
	}
	else
	{
		this->position.y += overlapY;
		if (overlapY > 0 || overlapY < 0)
		{
			this->velocity.y = 0;
		}

		if (overlapY > 0)
		{
			isGrounded = true;
		}
	}
}

void Entity2D::setHealth(unsigned int health)
{
	this->health = health;
}

unsigned int Entity2D::getHealth(void)
{
	return health;
}

void Entity2D::setHitboxSize(glm::vec2 hitboxSize)
{
	this->hitboxSize = hitboxSize;

	hitboxHalfSizeX = hitboxSize.x * 0.5f;
	hitboxHalfSizeY = hitboxSize.y * 0.5f;
}

glm::vec2 Entity2D::getHitboxSize(void)
{
	return this->hitboxSize;
}

void Entity2D::setHitboxOffset(glm::vec2 hitboxOffset)
{
	this->hitboxOffset = hitboxOffset;
}

glm::vec2 Entity2D::getHitboxOffset(void)
{
	return this->hitboxOffset;
}

glm::vec2 Entity2D::getForce(void)
{
	return force;
}

void Entity2D::setForce(glm::vec2 force)
{
	this->force = force;
}

bool Entity2D::getToDestroy(void)
{
	return toDestroy;
}

void Entity2D::setDoPhysics(bool doPhysics)
{
	this->isPhysics = doPhysics;
}

bool Entity2D::getDoPhysics(void)
{
	return isPhysics;
}

void Entity2D::setDoGravity(bool doGravity)
{
	this->isGravity = doGravity;
}

bool Entity2D::getDoGravity(void)
{
	return isGravity;
}

bool Entity2D::getXUpdate(void)
{
	return xUpdate;
}

bool Entity2D::getYUpdate(void)
{
	return yUpdate;
}
