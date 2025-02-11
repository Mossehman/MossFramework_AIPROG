#pragma once
#include <BaseLevel.h>
class MazeMap : public BaseLevel
{
public:
	MazeMap(unsigned int MapX, unsigned int MapY, glm::vec2 TileSize, PerlinNoiseConfig* Config1D, PerlinNoiseConfig* Config2D);
	glm::vec2 GetStartPos();
	void InitialiseSpawn(glm::vec2 startPos, float minPercentageAir);

protected:
	void NoiseToTiles2D(int width, int height, const float* Input, std::vector<std::vector<int>>& noiseMap) override;
	void InitTiles() override;
	void GeneratePerlinMap(std::vector<std::vector<int>> firstMap, std::vector<std::vector<int>> otherMap) override;

	PerlinNoiseConfig* Config2D;

	void Floodfill(glm::ivec2 startIndex, std::vector<glm::ivec2>& reachableTiles);
	glm::vec2 startPos;

};

