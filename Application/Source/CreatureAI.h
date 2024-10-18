#pragma once
#include "Entity2D.h"
#include "Map2D.h"
//#include "pathNode.h"
#include "StateManager.h"



class CreatureAI : public Entity2D //creatures are an entity
{
protected:
	glm::vec2 targetPosition; //this is the position to pathfind to
	bool calculatePath; //check for when we want to recalculate the astar list
	glm::vec2 aStarRange;

	std::vector<glm::vec2> waypointList;
	int waypointIndex;

	//pathNode* nodes = nullptr;
	//pathNode* startNode = nullptr;
	//pathNode* endNode = nullptr;

	StateManager* finiteStateMachine;

	float moveSpeed;
	float jumpForce;

	bool reachedXdest, reachedYdest;


public:
	CreatureAI(int entityLevel, glm::vec2 pos = glm::vec2(0, 0), glm::vec2 rot = glm::vec2(0, 0), glm::vec2 scl = glm::vec2(1, 1));
	CreatureAI();
	~CreatureAI();

	virtual void Init(bool isGravity = true, bool isPhysics = true, bool checkTileCollisions = true, bool entityCollision = true);
	virtual void Update(double dt);

	void doTileCollision();
	void recalculateNodes();

	glm::vec2 getTargetPos(void);
	void setTargetPos(glm::vec2 targetPos);

	bool toCalculatePath(void);
	void setCalculatePath(bool calculatePath);

	std::vector<glm::vec2>& getWaypointList(void);
	void addToWaypointList(glm::vec2 newWaypoint);

	unsigned int getWaypointIndex(void);
	void setWaypointIndex(unsigned int waypointIndex);

	int getCurrentLevel(void);
	void setCurrentLevel(int currentLevel);
	
	void solveAStar();
	void getCurrentPath();
	void renderPath();

	virtual void pathFind(double dt);

	void renderEntityNode(float size = 30.f);
	void renderTargetNode(float size = 30.f);

	virtual void initialiseStates() {}
	virtual void updateFSM(double dt) {}

	void setMoveSpeed(float moveSpeed);
	float getMoveSpeed(void);
};

