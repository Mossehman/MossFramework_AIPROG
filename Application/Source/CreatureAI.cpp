#include "CreatureAI.h"
#include <list>

CreatureAI::CreatureAI(int entityLevel, glm::vec2 pos, glm::vec2 rot, glm::vec2 scl)
{
    nodes = new pathNode[Map2D::GetInstance()->mapSizeX[entityLevel] * Map2D::GetInstance()->mapSizeY[entityLevel]];
    finiteStateMachine = new StateManager();

    moveSpeed = 20.f;
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
            if (!Map2D::GetInstance()->getMapDangeous(y, x, currentLevel) || Map2D::GetInstance()->getMapDangeous(y, x, currentLevel))
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
            { nodes[y * Map2D::GetInstance()->mapSizeX[entityLevel] + x].addToNeighbourNodes(&nodes[(y - 1) * Map2D::GetInstance()->mapSizeX[entityLevel] + x]); }
            
            if (y < Map2D::GetInstance()->mapSizeY[entityLevel] - 1) 
            { nodes[y * Map2D::GetInstance()->mapSizeX[entityLevel] + x].addToNeighbourNodes(&nodes[(y + 1) * Map2D::GetInstance()->mapSizeX[entityLevel] + x]); }


            if (x > 0) 
            { nodes[y * Map2D::GetInstance()->mapSizeX[entityLevel] + x].addToNeighbourNodes(&nodes[y * Map2D::GetInstance()->mapSizeX[entityLevel] + (x - 1)]); }

            if (x < Map2D::GetInstance()->mapSizeX[entityLevel] - 1)
            { nodes[y * Map2D::GetInstance()->mapSizeX[entityLevel] + x].addToNeighbourNodes(&nodes[y * Map2D::GetInstance()->mapSizeX[entityLevel] + (x + 1)]); }
        }
    }

    startNode = &nodes[(int)Map2D::GetInstance()->posToTilePos(position).y * Map2D::GetInstance()->mapSizeY[entityLevel] + (int)Map2D::GetInstance()->posToTilePos(position + (Map2D::GetInstance()->getTileSize().x * 0.5f)).x];
}

CreatureAI::~CreatureAI()
{
    if (waypointList.size() > 0) { waypointList.clear(); }
    if (nodes) { delete nodes; }
    if (startNode) { delete startNode; }
    if (endNode) { delete endNode; }
    if (finiteStateMachine) { delete finiteStateMachine; }
}

CreatureAI::CreatureAI()
{
}

void CreatureAI::Init(bool isGravity, bool isPhysics, bool checkTileCollisions, bool entityCollision)
{
    this->isGravity = isGravity;
    this->isPhysics = isPhysics;
    this->checkTileCollisions = checkTileCollisions;
    this->entityCollision = entityCollision;

    initialiseStates();
}

void CreatureAI::Update(double dt)
{
    if (currentLevel != Map2D::GetInstance()->getCurrentLevel() && !updateInBackend) { return; } 
    if (this->health <= 0) 
    {
        toDestroy = true;
    }
    //isGrounded = false;

    if (Map2D::GetInstance()->mapUpdate)
    {
        recalculateNodes();
    }

    checkMinMaxPos(); 
    doGravity(gravityValue); 
    //pathFind(dt);
    doPhysics(dt); 
    checkPosUpdate(); 
    doTileCollision();
    updateFSM(dt);
}

glm::vec2 CreatureAI::getTargetPos(void)
{
    return targetPosition;
}

void CreatureAI::setTargetPos(glm::vec2 targetPos)
{
    this->targetPosition = targetPos;
    endNode = &nodes[(int)Map2D::GetInstance()->posToTilePos(targetPos).y * Map2D::GetInstance()->mapSizeY[currentLevel] + (int)Map2D::GetInstance()->posToTilePos(targetPos + (Map2D::GetInstance()->getTileSize().x * 0.5f)).x];
}

void CreatureAI::doTileCollision()
{
    float resolutionX = std::numeric_limits<float>::max(), resolutionY = std::numeric_limits<float>::max();
    float resolutionXmin = std::numeric_limits<float>::max(), resolutionXmax = std::numeric_limits<float>::max();
    float resolutionYmin = std::numeric_limits<float>::max(), resolutionYmax = std::numeric_limits<float>::max();

    bool isCollidingX = false, isCollidingY = false;

    // Check for X-axis collisions
    if (position.x + hitboxOffset.x - hitboxSize.x * 0.5f < minPos.x)
    {
        resolutionXmin = minPos.x - (position.x + hitboxOffset.x - hitboxSize.x * 0.5f);
        isCollidingX = true;
    }
    if (position.x + hitboxOffset.x + hitboxSize.x * 0.5f > maxPos.x)
    {
        resolutionXmax = maxPos.x - (position.x + hitboxOffset.x + hitboxSize.x * 0.5f);
        isCollidingX = true;
    }

    if (isCollidingX)
    {
        resolutionX = (std::abs(resolutionXmin) < std::abs(resolutionXmax)) ? resolutionXmin : resolutionXmax;
    }

    // Check for Y-axis collisions
    if (position.y + hitboxOffset.y - hitboxSize.y * 0.5f < minPos.y)
    {
        resolutionYmin = minPos.y - (position.y + hitboxOffset.y - (hitboxSize.y * 0.5f));
        isCollidingY = true;
    }
    if (position.y + hitboxOffset.y + hitboxSize.y * 0.5f > maxPos.y)
    {
        resolutionYmax = maxPos.y - (position.y + hitboxOffset.y + (hitboxSize.y * 0.5f));
        isCollidingY = true;
    }

    if (isCollidingY)
    {
        resolutionY = (std::abs(resolutionYmin) < std::abs(resolutionYmax)) ? resolutionYmin : resolutionYmax;
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
        }
    }
}

void CreatureAI::recalculateNodes()
{
    if (!Map2D::GetInstance()->mapUpdate) { return; } //if map was not updated, we do not need to recalculate the nodes

    //update only the nodes on the modified tiles
    for (int y = 0; y < Map2D::GetInstance()->updatedTileYindex.size(); y++)
    {
        for (int x = 0; x < Map2D::GetInstance()->updatedTileXindex.size(); x++)
        {
            //get the index of the node by multiplying the y value with the width and then adding the x value
            int index = Map2D::GetInstance()->updatedTileYindex[y] * Map2D::GetInstance()->mapSizeX[currentLevel] + Map2D::GetInstance()->updatedTileYindex[x];
            nodes[index].setPassable(true); 
            if (!Map2D::GetInstance()->getMapPassable(y, x, currentLevel) || Map2D::GetInstance()->getMapDangeous(y, x, currentLevel))
            {
                //if tile to check is solid or is a dangerous tile, false
                nodes[index].setPassable(false);
            }
            nodes[index].setCheckedStatus(false);
        }
    }

    //this way, we do not need to recompute all the nodes across the map, and instead can optimise it to only update nodes at modified tiles
    //this should prevent the framedrops when tiles are modified
}

bool CreatureAI::toCalculatePath(void)
{
    return calculatePath;
}

void CreatureAI::setCalculatePath(bool calculatePath)
{
    this->calculatePath = calculatePath;
}

std::vector<glm::vec2>& CreatureAI::getWaypointList(void)
{
    return waypointList;
}

void CreatureAI::addToWaypointList(glm::vec2 newWaypoint)
{
    waypointList.push_back(newWaypoint);
}

unsigned int CreatureAI::getWaypointIndex(void)
{
    return waypointIndex;
}

void CreatureAI::setWaypointIndex(unsigned int waypointIndex)
{
    //attempting to set the waypoint index to a value out of range
    if (waypointIndex > waypointList.size() - 1) { return; } 

    this->waypointIndex = waypointIndex;
}

int CreatureAI::getCurrentLevel(void)
{
    return currentLevel;
}

void CreatureAI::setCurrentLevel(int creatureLevel)
{
    this->currentLevel = creatureLevel;
}

void CreatureAI::solveAStar()
{
    for (int x = 0; x < Map2D::GetInstance()->mapSizeX[currentLevel]; x++)
    {
        for (int y = 0; y < Map2D::GetInstance()->mapSizeY[currentLevel]; y++)
        {
            int index = Map2D::GetInstance()->mapSizeX[currentLevel] * y + x;
            nodes[index].setCheckedStatus(false);
            nodes[index].setGlobalGoal(std::numeric_limits<float>::max());
            nodes[index].setLocalGoal(std::numeric_limits<float>::max());
            nodes[index].setParentNode(nullptr);
        }
    }

    startNode = &nodes[(int)Map2D::GetInstance()->posToTilePos(position).y * Map2D::GetInstance()->mapSizeY[currentLevel] + (int)Map2D::GetInstance()->posToTilePos(position).x];

    auto distanceBetweenNodes = [](pathNode* a, pathNode* b)
    {
        return sqrtf((a->getX() - b->getX()) * (a->getX() - b->getX()) + (a->getY() - b->getY()) * (a->getY() - b->getY()));
    };

    auto heuristic = [distanceBetweenNodes](pathNode* a, pathNode* b)
    {
        return distanceBetweenNodes(a, b);
    };

    pathNode* currentNodeToCheck = startNode;
    startNode->setLocalGoal(0.0f);
    startNode->setGlobalGoal(heuristic(startNode, endNode));

    std::list<pathNode*> uncheckedNodeList;
    uncheckedNodeList.push_back(startNode);

    while (!uncheckedNodeList.empty() && currentNodeToCheck != endNode)
    {
        //sort list into ascending order
        uncheckedNodeList.sort([](const pathNode* lhs, const pathNode* rhs) {return lhs->getGlobalGoal() < rhs->getGlobalGoal(); });
        //if our lowest heuristic value has been checked already, remove it from the list
        while (!uncheckedNodeList.empty() && uncheckedNodeList.front()->getCheckedStatus())
        {
            uncheckedNodeList.pop_front();
        }

        //if list is empty, exit the loop
        if (uncheckedNodeList.empty()) { break; }

        currentNodeToCheck = uncheckedNodeList.front();
        currentNodeToCheck->setCheckedStatus(true);

        for (auto nodeNeighbour : currentNodeToCheck->getNeighbourNodes())
        {
            if (!nodeNeighbour->getCheckedStatus() && nodeNeighbour->getPassable())
            {
                uncheckedNodeList.push_back(nodeNeighbour);
            }

            float lowerGoalCheck = currentNodeToCheck->getLocalGoal() + distanceBetweenNodes(currentNodeToCheck, nodeNeighbour);

            if (lowerGoalCheck < nodeNeighbour->getLocalGoal())
            {
                nodeNeighbour->setParentNode(currentNodeToCheck);
                nodeNeighbour->setLocalGoal(lowerGoalCheck);
                nodeNeighbour->setGlobalGoal(nodeNeighbour->getLocalGoal() + heuristic(nodeNeighbour, endNode));
            }
        }
    }
}

void CreatureAI::getCurrentPath()
{
    if (endNode != nullptr)
    {
        waypointList.clear();
        pathNode* p = endNode;
        while (p->getParentNode())
        {
            waypointList.push_back(glm::vec2(p->getX(), p->getY()));
            p = p->getParentNode();
        }
        //reachedXdest = false;
        //reachedYdest = false;
        waypointIndex = waypointList.size() - 1;
    }
}

void CreatureAI::renderPath()
{
    for (int i = waypointList.size() - 1; i > 0; i--)
    {
        GameObject* pathRenderer = new GameObject();
        pathRenderer->setMesh(MeshBuilder::GenerateLineToLine("Path Render", Color(1.f, 0.f, 1.f), waypointList[i], waypointList[i - 1]));
        pathRenderer->Render();
        delete pathRenderer;
    }
}

void CreatureAI::pathFind(double dt)
{
    if (waypointIndex < 0) { return; }


    if (!reachedXdest)
    {
        if (position.x > waypointList[waypointIndex].x)
        {
            if (position.x - waypointList[waypointIndex].x < moveSpeed * dt)
            {
                position.x = waypointList[waypointIndex].x;
                reachedXdest = true;
            }
            else
            {
                position.x -= moveSpeed * dt;
            }
        }
        else if (position.x < waypointList[waypointIndex].x)
        {
            if (waypointList[waypointIndex].x - position.x < moveSpeed * dt)
            {
                position.x = waypointList[waypointIndex].x;
                reachedXdest = true;
            }
            else
            {
                position.x += moveSpeed * dt;
            }
        }
        else
        {
            reachedXdest = true;
        }
    }

    if (!isGravity)
    {
        
        if (!reachedYdest)
        {
            if (position.y > waypointList[waypointIndex].y)
            {
                if (position.y - waypointList[waypointIndex].y < moveSpeed * dt)
                {
                    position.y = waypointList[waypointIndex].y;
                    reachedYdest = true;
                }
                else
                {
                    position.y -= moveSpeed * dt;
                }
            }
            else if (position.y < waypointList[waypointIndex].y)
            {
                if (waypointList[waypointIndex].y - position.y < moveSpeed * dt)
                {
                    position.y = waypointList[waypointIndex].y;
                    reachedYdest = true;
                }
                else
                {
                    position.y += moveSpeed * dt;
                }
            }
            else
            {
                reachedYdest = true;
            }
        }

        if (reachedXdest && reachedYdest)
        {
            waypointIndex--;
            reachedXdest = false;
            reachedYdest = false;
        }
    }
    else
    {

        if (position.y < waypointList[waypointIndex].y && isGrounded)
        {
            this->force.y += jumpForce;
            isGrounded = false;
        }

        if (reachedXdest)
        {
            waypointIndex--;
            reachedXdest = false;
        }
    }



}

void CreatureAI::renderEntityNode(float size)
{
    GameObject* newObj = new GameObject();
    newObj->setPosition(glm::vec2(startNode->getX(), startNode->getY()));
    newObj->setScale(glm::vec2(size, size));
    newObj->setMesh(MeshBuilder::GenerateQuad("entity node mesh", Color(1.f, 0.f, 0.f)));
    newObj->Render();
    delete newObj;
}

void CreatureAI::renderTargetNode(float size)
{
    GameObject* newObj = new GameObject();
    newObj->setPosition(glm::vec2(endNode->getX(), endNode->getY()));
    newObj->setScale(glm::vec2(size, size));
    newObj->setMesh(MeshBuilder::GenerateQuad("target node mesh", Color(0.f, 1.f, 0.f)));
    newObj->Render();
    delete newObj;
}

void CreatureAI::setMoveSpeed(float moveSpeed)
{
    this->moveSpeed = moveSpeed;
}

float CreatureAI::getMoveSpeed(void)
{
    return moveSpeed;
}
