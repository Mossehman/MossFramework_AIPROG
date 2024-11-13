#include "EntityAI2D.h"
#include "Map2D.h"

#include <list>

void EntityAI2D::InitAStar()
{
	AStar* AStarNodes = Map2D::GetInstance()->GetAStar();
	pathData.resize(AStarNodes->GetNodes().size());

	//since our nodes won't be changing position, we can set the pathData reference once in our init function

	for (int i = 0; i < AStarNodes->GetNodes().size(); i++)
	{
		pathData[i]->currNode = AStarNodes->GetNodes()[i];
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

	int startIndex = (int)Map2D::GetInstance()->PosToTilePos(position).y * Map2D::GetInstance()->GetLevel()->GetMapY() + (int)Map2D::GetInstance()->PosToTilePos(position).x;
	int endIndex = (int)Map2D::GetInstance()->PosToTilePos(targetPos).y * Map2D::GetInstance()->GetLevel()->GetMapY() + (int)Map2D::GetInstance()->PosToTilePos(targetPos).x;

	//calculate the start and end nodes (can use as local variables since we just need to cache the waypoint list and then delete them after)
	PathNode* startNode = AStarNodes->GetNodes()[startIndex];
	PathNode* endNode = AStarNodes->GetNodes()[endIndex];
	destinationData = pathData[endIndex];
	destinationData->currNode = endNode;

	//lambda functions to calculate the distance between 2 nodes and return the heuristic for calculating weightage
	auto distanceBetweenNodes = [](PathNode* a, PathNode* b)
	{
		return sqrtf((a->Position.x - b->Position.x) * (a->Position.x - b->Position.x) + (a->Position.y - b->Position.y) * (a->Position.y - b->Position.y));
	};

	auto heuristic = [distanceBetweenNodes](PathNode* a, PathNode* b)
	{
		return distanceBetweenNodes(a, b);
	};

	int currentCheckIndex = startIndex;
	pathData[currentCheckIndex]->localGoal = 0.0f;
	pathData[currentCheckIndex]->globalGoal = heuristic(startNode, endNode);

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

			float lowerGoalCheck = pathData[currentCheckIndex]->localGoal + distanceBetweenNodes(AStarNodes->GetNodes()[currentCheckIndex], AStarNodes->GetNodes()[checkIndex]);

			if (lowerGoalCheck < pathData[checkIndex]->localGoal)
			{
				//set a reference to the neighbouring node
				pathData[checkIndex]->ParentIndex = checkIndex;
				pathData[checkIndex]->localGoal = lowerGoalCheck;
				pathData[checkIndex]->globalGoal = pathData[checkIndex]->globalGoal + heuristic(AStarNodes->GetNodes()[checkIndex], endNode);
			}

		}

	}

	if (destinationData != nullptr)
	{
		pathWaypoints.clear();
		int dat = endIndex;

		while (AStarNodes->GetNodes()[pathData[dat]->ParentIndex] != nullptr)
		{
			pathWaypoints.push_back(glm::vec2(pathData[dat]->currNode->Position.x, pathData[dat]->currNode->Position.y));
			dat = pathData[dat]->ParentIndex;
		}

		currWaypointIndex = pathWaypoints.size() - 1;
	}


}

void EntityAI2D::Notify(std::string messageData, int priority)
{
	if (priority > currentMsgPriority)
	{
		currentMsg = messageData;
	}
}

void EntityAI2D::InitialiseStates()
{
	for (int i = 0; i < finiteStates.size(); i++)
	{
		finiteStates[i]->Init(this);
	}
}

void EntityAI2D::AddState(FiniteState* newState)
{
	if (!newState) { return; } //check if the state we want to add is Null

	for (int i = 0; i < finiteStates.size(); i++) //loop through all currently added states
	{
		if (newState->GetName() == finiteStates[i]->GetName()) //if newState's name is equal to the currently checked name
		{
			std::cout << "Duplicate State Name detected: " << newState->GetName() << ", unable to add!" << std::endl; //output error value
			return; //stop program here
		}
	}

	finiteStates.push_back(newState); //if no duplicate name detected, add to the list
}

void EntityAI2D::RemoveState(std::string stateName)
{
	for (int i = 0; i < finiteStates.size(); i++)
	{
		if (finiteStates[i]->GetName() == stateName)
		{
			finiteStates.erase(finiteStates.begin() + i); //if state is found, delete state from the list
			return;
		}
	}

	std::cout << "State with name: " << stateName << " removed!" << std::endl;
}

std::vector<FiniteState*> EntityAI2D::GetStates(void)
{
	return finiteStates;
}

FiniteState* EntityAI2D::GetCurrentState(void)
{
	return currentState;
}

void EntityAI2D::SetCurrentState(FiniteState* newCurrentState)
{
	currentState->OnExit(); //Run the exit code for the current state
	newCurrentState->OnEnter(); //Run the entry code for the newly added state
	currentState = newCurrentState; //Set the state passed in via param as the new Current State
}

int EntityAI2D::GetMessagePriority(void)
{
	return currentMsgPriority;
}

std::string EntityAI2D::GetMessageData(void)
{
	return currentMsg;
}
