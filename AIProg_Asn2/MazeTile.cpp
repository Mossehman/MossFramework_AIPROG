#include "MazeTile.h"

MazeTile::MazeTile()
{
	for (int i = 0; i < MAX_TILE_CONNECTIONS; i++)
	{
		adjacentTiles[i] = nullptr;
	}
}
