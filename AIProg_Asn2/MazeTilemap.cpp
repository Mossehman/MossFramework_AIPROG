//#include "MazeTilemap.h"
//#include <stack>
//#include <vector>
//#include <random>
//
//glm::ivec2 MazeTilemap::posToChunk(glm::ivec2 tileIndex)
//{
//	float x = (float)tileIndex.x / chunkHalfSize.x;
//	float y = (float)tileIndex.y / chunkHalfSize.y;
//
//	int xIndex, yIndex;
//
//	if (x < 0) {
//		xIndex = std::ceil(x);
//	}
//	else {
//		xIndex = std::floor(x);
//	}
//
//	if (y < 0) {
//		yIndex = std::ceil(y);
//	}
//	else {
//		yIndex = std::floor(y);
//	}
//
//	return glm::ivec2(xIndex, yIndex);
//}
//
//void MazeTilemap::GenerateChunk(MazeTile* startTile)
//{
//	glm::ivec2 currChunk = posToChunk(startTile->tilePosition);
//
//	for (int i = 0; i < MAX_TILE_CONNECTIONS; i++) {
//		//when checking the neighbouring tiles of the starting tile, if they are null or have been discovered, return
//		if (!startTile->adjacentTiles[i] || startTile->adjacentTiles[i]->IsDiscovered) { continue; }
//		glm::ivec2 chunkToCheck = posToChunk(startTile->adjacentTiles[i]->tilePosition);
//		//if this discovered tile is in the same chunk as our tile, do not generate the chunk (this should not happen unless we do something really stupid)
//		if (chunkToCheck.x == currChunk.x && chunkToCheck.y == currChunk.y) { continue; }
//
//		std::stack<MazeTile*> tiles;
//		tiles.push(startTile->adjacentTiles[i]);
//
//		MazeTile* currTile = startTile->adjacentTiles[i];
//
//		while (!tiles.empty())
//		{
//
//			//while loop to backtrack from the stack
//			while (!tiles.empty()) {
//				//this tile is out of bounds, remove it from the stack
//				if (!tiles.top()->IsDiscovered) {
//					tiles.pop();
//					continue;
//				}
//				bool toBreak = false;
//
//				for (int j = 0; j < MAX_TILE_CONNECTIONS; j++)
//				{
//					//if there is an available connection
//					if (!tiles.top()->adjacentTiles[j]) { 
//						//check if that connection point has already been occupied by some other tile in our tilemap
//						glm::ivec2 tilePos = tiles.top()->tilePosition;
//						if ((j == 0 && tilemapData.count(glm::ivec2(tilePos.x, tilePos.y - 1)) > 0) ||
//							(j == 1 && tilemapData.count(glm::ivec2(tilePos.x + 1, tilePos.y)) > 0) ||
//							(j == 2 && tilemapData.count(glm::ivec2(tilePos.x, tilePos.y + 1)) > 0) ||
//							(j == 3 && tilemapData.count(glm::ivec2(tilePos.x - 1, tilePos.y)) > 0)) {
//							continue;
//						}
//						//if there was an open space, tell the code that it should break out of the while loop
//						toBreak = true;
//						break;
//					}
//				}
//
//				if (toBreak) { break; }
//				tiles.pop();
//			}
//
//			if (tiles.empty()) { break; }
//
//
//
//
//
//		}
//
//		break;
//	}
//}
