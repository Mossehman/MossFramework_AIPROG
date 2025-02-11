#pragma once
#include <GameObject.h>
#define MAX_TILE_CONNECTIONS 4

class MazeTile
{
public:
	MazeTile* adjacentTiles[MAX_TILE_CONNECTIONS];
	int tileID = 0;
	glm::ivec2 tilePosition;
	bool IsDiscovered = false;

	MazeTile();

};

