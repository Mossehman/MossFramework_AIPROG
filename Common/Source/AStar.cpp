#include "AStar.h"
#include <iostream>

AStar::AStar(glm::vec2 tileSize)
{
    this->tileSize = tileSize;
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


            newNode->ParentNode = nullptr;

            Nodes.push_back(newNode);
        }
    }

    for (int x = xStart; x < xEnd; x++)
    {
        for (int y = yStart; y < yEnd; y++)
        {
            if (y > 0)
            {
                Nodes[y * (xEnd + xStart) + x]->NeighbouringNodes.push_back(Nodes[(y - 1) * (xEnd + xStart) + x]);
            }

            if (y < (yEnd + yStart) - 1)
            {
                Nodes[y * (xEnd + xStart) + x]->NeighbouringNodes.push_back(Nodes[(y + 1) * (xEnd + xStart) + x]);
            }

            if (x > 0)
            {
                Nodes[y * (xEnd + xStart) + x]->NeighbouringNodes.push_back(Nodes[y * (xEnd + xStart) + (x - 1)]);
            }

            if (x < (xEnd + xStart) - 1)
            {
                Nodes[y * (xEnd + xStart) + x]->NeighbouringNodes.push_back(Nodes[y * (xEnd + xStart) + (x + 1)]);
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
