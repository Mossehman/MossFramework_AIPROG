#pragma once
#include "GameObject.h"

class Entity2D : public GameObject
{
protected:

	bool xUpdate = false;
	bool yUpdate = false;

	glm::vec2 force, acceleration, velocity, iVelocity;

	glm::vec2 originalPos;

	float mass = 60;
	float gravityScale = 1;
	float roughness = 1;

	bool isGravity = true;
	bool isPhysics = true;
	bool isGrounded;
	bool checkTileCollisions;
	bool entityCollision;

	bool foundBorderX, foundBorderY;

	glm::vec2 minPos, maxPos;
	glm::vec2 hitboxSize;
	glm::vec2 hitboxOffset;

	int health;
	bool toDestroy;


	float hitboxHalfSizeX;
	float hitboxHalfSizeY;
	
	glm::vec2 cachedMinPos;
	glm::vec2 cachedMaxPos;

public:

	float friction = 1.f;
	glm::vec2 gravityValue;

	Entity2D(glm::vec2 pos = glm::vec2(0, 0), glm::vec2 rot = glm::vec2(0, 0), glm::vec2 scl = glm::vec2(1, 1));

	virtual void Init(std::vector<GameObject*>& objList, int currentLevel, bool isGravity = true, bool isPhysics = true, bool checkTileCollisions = true, bool entityCollision = true);
	virtual void Update(double dt);
	
	//Checks for tilemap collisions and sets the entity's min and max positions accordingly
	void checkMinMaxPos();
	void checkBorderX();
	void checkBorderY();

	void resetBorderX();
	void resetBorderY();

	virtual void doTileCollision();


	void doPhysics(double dt, float airResistance = 1.f, float timeScale = 1.f);
	void doGravity(glm::vec2 gravity);

	void debugMove(float speed, double dt);

	void setHealth(unsigned int health);
	unsigned int getHealth(void);

	void checkPosUpdate();

	bool checkEntityCollision(Entity2D* entityToCheck);
	void resolveEntityCollision(Entity2D* entityToCheck);

	void setHitboxSize(glm::vec2 hitboxSize);
	glm::vec2 getHitboxSize(void);

	void setHitboxOffset(glm::vec2 hitboxOffset);
	glm::vec2 getHitboxOffset(void);

	glm::vec2 getForce(void);
	void setForce(glm::vec2 force);

	bool getToDestroy(void);
	

	void setDoPhysics(bool doPhysics);
	bool getDoPhysics(void);

	void setDoGravity(bool doGravity);
	bool getDoGravity(void);

	void setDoTileCollision(bool tileCollision);
	bool getDoTileCollision(void);

	void setDoEntityCollision(bool entityCollision);
	bool getDoEntityCollision(void);

	bool getXUpdate(void);
	bool getYUpdate(void);
};

/*

- we only want to calculate the min and max X when the y level changes
- we want to loop based on hitbox size

*/

