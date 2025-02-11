#include <glm/glm.hpp>
#include <map>
#include "MazeTile.h"
#include <DesignPatterns/SingletonTemplate.h>

class MazeTilemap : public SingletonTemplate<MazeTilemap>
{
private:
	std::map<glm::ivec2, MazeTile> tilemapData;
	glm::vec2 tileSize;
	
	std::map<int, bool> tilePassability;
	std::map<int, int> tileCost;

public:
	static MazeTile undiscoveredTile;
	glm::ivec2 chunkHalfSize = glm::ivec2(15, 15);
	glm::ivec2 posToChunk(glm::ivec2 tileIndex);

	void GenerateChunk(MazeTile* startTile);
	void Render();

};
