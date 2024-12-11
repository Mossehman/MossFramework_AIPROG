#include "EntityAI2D.h"
#include "EntityAI2D.h"
#include "Map2D.h"

#include <list>
#include <glm/gtx/norm.hpp>

void EntityAI2D::InitAStar()
{
	AStar* AStarNodes = Map2D::GetInstance()->GetAStar();
	pathData.resize(AStarNodes->GetNodes().size());

	//since our nodes won't be changing position, we can set the pathData reference once in our init function

	for (int i = 0; i < AStarNodes->GetNodes().size(); i++)
	{
		EntityPathData* newData = new EntityPathData();
		newData->currNode = AStarNodes->GetNodes()[i];
		pathData[i] = newData;
	}
}

void EntityAI2D::SolveAStar()
{
	AStar* AStarNodes = Map2D::GetInstance()->GetAStar();

    for (int y = 0; y < Map2D::GetInstance()->GetLevel()->GetMapY(); y++)
    {
        for (int x = 0; x < Map2D::GetInstance()->GetLevel()->GetMapX(); x++)
        {
            //convert the 2 dimensional array indexes into an index in 1 dimension
            int index = Map2D::GetInstance()->GetLevel()->GetMapX() * y + x;

            //reset the pathdata and node values
            pathData[index]->hasChecked = false;
            pathData[index]->globalGoal = std::numeric_limits<float>::max();
            pathData[index]->localGoal = std::numeric_limits<float>::max();
			pathData[index]->ParentIndex = -1;
        }
    }

	int mapWidth = Map2D::GetInstance()->GetLevel()->GetMapX(); // Number of columns in the map
	int startIndex = (int)Map2D::GetInstance()->PosToTilePos(position).y * mapWidth + (int)Map2D::GetInstance()->PosToTilePos(position).x;
	int endIndex = (int)Map2D::GetInstance()->PosToTilePos(targetPos).y * mapWidth + (int)Map2D::GetInstance()->PosToTilePos(targetPos).x;


	//calculate the start and end nodes (can use as local variables since we just need to cache the waypoint list and then delete them after)
	PathNode* startNode = AStarNodes->GetNodes()[startIndex];
	PathNode* endNode = AStarNodes->GetNodes()[endIndex];
	destinationData = pathData[endIndex];
	destinationData->currNode = endNode;

	//lambda functions to calculate the distance between 2 nodes and return the heuristic for calculating weightage
	auto distanceBetweenNodes = [](PathNode* a, PathNode* b, float nextNodeWeight)
	{
		return ((a->Position.x - b->Position.x) * (a->Position.x - b->Position.x) + (a->Position.y - b->Position.y) * (a->Position.y - b->Position.y)) * nextNodeWeight; 
	};

	auto heuristic = [distanceBetweenNodes](PathNode* a, PathNode* b, float nextNodeWeight)
	{
		return distanceBetweenNodes(a, b, nextNodeWeight);
	};

	int currentCheckIndex = startIndex;
	pathData[currentCheckIndex]->localGoal = 0.0f;
	pathData[currentCheckIndex]->globalGoal = heuristic(startNode, endNode, destinationData->NodeWeight);

	std::list<int> uncheckedPathIndexes;
	uncheckedPathIndexes.push_back(currentCheckIndex);

	while (!uncheckedPathIndexes.empty() && currentCheckIndex != endIndex)
	{
		uncheckedPathIndexes.sort([&](int lhsIndex, int rhsIndex) { return pathData[lhsIndex]->globalGoal < pathData[rhsIndex]->globalGoal; });

		while (!uncheckedPathIndexes.empty() && pathData[uncheckedPathIndexes.front()]->hasChecked)
		{
			uncheckedPathIndexes.pop_front();
		}
		if (uncheckedPathIndexes.empty()) { break; }

		currentCheckIndex = uncheckedPathIndexes.front();
		pathData[currentCheckIndex]->hasChecked = true;

		for (int checkIndex : AStarNodes->GetNodes()[currentCheckIndex]->NeighbouringNodesIndex)
		{
			if (!pathData[checkIndex]->hasChecked && AStarNodes->GetNodes()[checkIndex]->Passability == 0)
			{
				uncheckedPathIndexes.push_back(checkIndex);
			}

			float lowerGoalCheck = pathData[currentCheckIndex]->localGoal +
				distanceBetweenNodes(AStarNodes->GetNodes()[currentCheckIndex],
					AStarNodes->GetNodes()[checkIndex], pathData[checkIndex]->NodeWeight) +
				pathData[checkIndex]->NodeWeight;

			if (lowerGoalCheck < pathData[checkIndex]->localGoal)
			{
				//set a reference to the neighbouring node
				pathData[checkIndex]->ParentIndex = currentCheckIndex;
				pathData[checkIndex]->localGoal = lowerGoalCheck;
				pathData[checkIndex]->globalGoal = pathData[checkIndex]->globalGoal + heuristic(AStarNodes->GetNodes()[checkIndex], endNode, pathData[checkIndex]->NodeWeight);
			}

		}

	}

	if (destinationData != nullptr)
	{
		pathWaypoints.clear();
		int dat = endIndex;

		while (pathData[dat]->ParentIndex != -1 && AStarNodes->GetNodes()[pathData[dat]->ParentIndex] != nullptr)
		{
			pathWaypoints.push_back(glm::vec2(pathData[dat]->currNode->Position.x, pathData[dat]->currNode->Position.y));
			dat = pathData[dat]->ParentIndex;
		}

		currWaypointIndex = pathWaypoints.size() - 1;
	}
}

void EntityAI2D::MoveAlongPath(float dt)
{
	if (pathWaypoints.size() == 1)
	{ 
		pathWaypoints.pop_back();
		return;
	}
	else if (pathWaypoints.size() == 0)
	{
		return;
	}

	float Movement = moveSpeed * MoveSpeedMultiplier * dt;
	if (glm::distance2(position, pathWaypoints[pathWaypoints.size() - 1]) < Movement * Movement)
	{
		position = pathWaypoints[pathWaypoints.size() - 1];
		pathWaypoints.pop_back();
		return;
	}

	glm::vec2 dir = glm::normalize(pathWaypoints[pathWaypoints.size() - 1] - position);

	if (pathWaypoints[pathWaypoints.size() - 1].x < position.x)
	{
		this->scale.x = -1 * cachedScale.x;
	}
	else
	{
		this->scale.x = cachedScale.x;
	}

	position += dir * Movement;
}

void EntityAI2D::RenderNodePath(Color PathColor)
{
	if (pathWaypoints.size() <= 0) { return; }

	for (int i = 0; i < pathWaypoints.size(); i++)
	{
		if (i == pathWaypoints.size() - 1) { break; }

		GameObject* lineObj = new GameObject();
		lineObj->SetMesh(MeshBuilder::GenerateLineDir("Line", PathColor, pathWaypoints[i], pathWaypoints[i + 1]));
		lineObj->Render();
		delete lineObj;
	}

	GameObject* lineObj = new GameObject();
	lineObj->SetMesh(MeshBuilder::GenerateLineDir("Line", PathColor, pathWaypoints[pathWaypoints.size() - 1], position));
	lineObj->Render();
	delete lineObj;
}

EntityAI2D::EntityAI2D(glm::vec2 pos, glm::vec2 rot, glm::vec2 scl) : Entity2D(pos, rot, scl)
{
	stateMachine = new FiniteStateMachine(this);
}

FiniteStateMachine*& EntityAI2D::GetFSM()
{
	return this->stateMachine;
}

void EntityAI2D::ResetPathWeight()
{
	for (EntityPathData* entityPathDat : pathData)
	{
		entityPathDat->NodeWeight = 1.0f;
	}
}

void EntityAI2D::SetPathWeightage(float newWeight, int x, int y)
{
	int mapWidth = Map2D::GetInstance()->GetLevel()->GetMapX();
	int index = y * mapWidth + x;

	if (index >= pathData.size() || index < 0) { return; }
	pathData[index]->NodeWeight = newWeight;
}

int EntityAI2D::GetPathSize()
{
	return pathWaypoints.size();
}
