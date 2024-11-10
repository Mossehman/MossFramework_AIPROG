#include "AStar.h"
#include <iostream>

AStar::AStar(glm::vec2 tileSize)
{
    this->tileSize = tileSize;
}

AStar::~AStar()
{
    for (int i = 0; i < Nodes.size(); i++)
    {
        delete Nodes[i]; //free up data from the nodes
    }
    Nodes.clear();
}

void AStar::Init(int xEnd, int yEnd, int xStart, int yStart)
{
    nodeCount = 0;
    if (xEnd <= xStart || yEnd <= yStart) { std::cout << "AStar Init For Loop values cannot run, start is greater than target end value!" << std::endl; return; }

    for (int x = xStart; x < xEnd; x++)
    {
        for (int y = yStart; y < yEnd; y++)
        {
            int index = y * (xEnd + xStart) + x;

            PathNode* newNode = new PathNode();

            newNode->Position.x = x * tileSize.x;
            newNode->Position.y = y * tileSize.y * -1;
            newNode->Passability = 0;

            Nodes.push_back(newNode);
        }
    }

    for (int x = xStart; x < xEnd; x++)
    {
        for (int y = yStart; y < yEnd; y++)
        {
            if (y > 0)
            {
                Nodes[y * (xEnd + xStart) + x]->NeighbouringNodesIndex.push_back((y - 1) * (xEnd + xStart) + x);
            }

            if (y < (yEnd + yStart) - 1)
            {
                Nodes[y * (xEnd + xStart) + x]->NeighbouringNodesIndex.push_back((y + 1) * (xEnd + xStart) + x);
            }

            if (x > 0)
            {
                Nodes[y * (xEnd + xStart) + x]->NeighbouringNodesIndex.push_back(y * (xEnd + xStart) + (x - 1));
            }

            if (x < (xEnd + xStart) - 1)
            {
                Nodes[y * (xEnd + xStart) + x]->NeighbouringNodesIndex.push_back(y * (xEnd + xStart) + (x + 1));
            }
        }
    }

}

std::vector<PathNode*>& AStar::GetNodes(void)
{
    return Nodes;
}

int AStar::GetNodeCount(void)
{
    return nodeCount;
}
