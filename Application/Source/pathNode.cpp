#include "pathNode.h"

bool pathNode::getPassable(void)
{
    return isPassable;
}

void pathNode::setPassable(bool isPassable)
{
    this->isPassable = isPassable;
}

bool pathNode::getCheckedStatus(void)
{
    return hasChecked;
}

void pathNode::setCheckedStatus(bool hasChecked)
{
    this->hasChecked = hasChecked;
}

float pathNode::getGlobalGoal(void) const
{
    return globalGoal;
}

void pathNode::setGlobalGoal(float globalGoal)
{
    this->globalGoal = globalGoal;
}

float pathNode::getLocalGoal(void)
{
    return localGoal;
}

void pathNode::setLocalGoal(float localGoal)
{
    this->localGoal = localGoal;
}

float pathNode::getX(void)
{
    return x;
}

void pathNode::setX(float x)
{
    this->x = x;
}

float pathNode::getY(void)
{
    return y;
}

void pathNode::setY(float y)
{
    this->y = y;
}

std::vector<pathNode*>& pathNode::getNeighbourNodes(void)
{
    return this->neighbourNodes;
}

void pathNode::addToNeighbourNodes(pathNode* nodeToAdd)
{
    neighbourNodes.push_back(nodeToAdd);
}

pathNode*& pathNode::getParentNode(void)
{
    return this->parentNode;
}

void pathNode::setParentNode(pathNode* parentNode)
{
    this->parentNode = parentNode;
}
