#include "MazeMap.h"
#include <Map2D.h>
#include <algorithm>

void MazeMap::NoiseToTiles2D(int width, int height, const float* Input, std::vector<std::vector<int>>& noiseMap)
{
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float tileID = (Input[y * width + x] * 12.0f);

			if (tileID >= 3.5 && tileID <= 4.5)
			{
				noiseMap[y][x] = 4; //place grass
			}
            else if (tileID >= 4 && tileID <= 5)
            {
                noiseMap[y][x] = 3; //place grass
            }

			else if (tileID >= 5 && tileID <= 7)
			{
				noiseMap[y][x] = 2; //place sand
			}
            else
            {
                noiseMap[y][x] = 1; //place stone
            }
        }
    }
}

void MazeMap::InitTiles()
{
    BindPassability(0, 0);
    BindTexture(1, "Image/stone.tga");
    BindPassability(1, 1);

    BindTexture(2, "Image/grass.jpg");
    BindPassability(2, 0);

	BindTexture(3, "Image/sand.tga");
	BindPassability(3, 0);
	SetTileCost(3, 2);

	BindTexture(4, "Image/water.png");
	BindPassability(4, 0);
	SetTileCost(4, 3);

	BindTexture(5, "Image/lava.png");
	BindPassability(5, 0);
	SetTileCost(5, 50);

	BindTexture(6, "Image/DD_elevator.png");
	BindPassability(6, 0);
}

void MazeMap::GeneratePerlinMap(std::vector<std::vector<int>> firstMap, std::vector<std::vector<int>> otherMap)
{
	tilemap.resize(MapSizeY);
	for (int y = 0; y < MapSizeY; y++)
	{
		tilemap[y].resize(MapSizeX);
		for (int x = 0; x < MapSizeX; x++)
		{
			Tile* newTile = new Tile(otherMap[y][x]);
			tilemap[y][x] = newTile;

		}
	}
}

void MazeMap::InitialiseSpawn(glm::vec2 startPos, float minPercentageAir)
{
	this->startPos = startPos;
	glm::ivec2 startingTileIndex = Map2D::GetInstance()->PosToTilePos(startPos);
	if (startingTileIndex.x < 0 || startingTileIndex.x >= MapSizeX ||
		startingTileIndex.y < 0 || startingTileIndex.y >= MapSizeY) {
		return;
	}

	//if in impassable tile, set tile to air
	if (tilemap[startingTileIndex.y][startingTileIndex.x]->Passability != 0) {
		tilemap[startingTileIndex.y][startingTileIndex.x]->tileID = 2;		
		tilemap[startingTileIndex.y][startingTileIndex.x]->Passability = 0;
		int AStarIndex = startingTileIndex.y * MapSizeX + startingTileIndex.x;
		Map2D::GetInstance()->GetAStar()->GetNodes()[AStarIndex]->weight = 1.0f;
	}

	tilemap[MapSizeY - 1][MapSizeX - 1]->tileID = 6;
	tilemap[MapSizeY - 1][MapSizeX - 1]->Passability = 0;
	
	for (int y = 0; y < MapSizeY; y++)
	{
		for (int x = 0; x < MapSizeX; x++)
		{
			if (tilemap[y][x]->Passability != 0) { continue; }
	
			std::cout << y << ", " << x << std::endl;

			glm::vec2 endPos = Map2D::GetInstance()->TileToPos(glm::vec2(x, y));
			std::vector<glm::vec2> pathList = Map2D::GetInstance()->GetPath(startPos, endPos, false, true);
			
			for (int i = 0; i < pathList.size(); i++)
			{

				glm::ivec2 tilePos = Map2D::GetInstance()->PosToTilePos(pathList[i]);
				if (tilemap[tilePos.y][tilePos.x]->Passability == 0) { continue; }
				tilemap[tilePos.y][tilePos.x]->tileID = 2;
				tilemap[tilePos.y][tilePos.x]->Passability = 0;

				int AStarIndex = tilePos.y * MapSizeX + tilePos.x;
				Map2D::GetInstance()->GetAStar()->GetNodes()[AStarIndex]->weight = 1.0f;
			
			}
		}

	}

	Init();
	Map2D::GetInstance()->GenerateNodes();
}

void MazeMap::ResetTiles()
{
	for (auto it = modifiedTiles.begin(); it != modifiedTiles.end(); ++it)
	{
		glm::ivec2 tileCoord = it->first;
		int ID = it->second;

		tilemap[tileCoord.y][tileCoord.x]->tileID = ID;
	}
	modifiedTiles.clear();

	Init();
	Map2D::GetInstance()->GenerateNodes();
}

void MazeMap::LavaEvent()
{
	std::srand(std::time(nullptr));
	std::vector<glm::vec2> grassTiles = GetTilesWithID(2);
	for (int i = 0; i < std::min(150, (int)grassTiles.size()); i++)
	{
		int randomIndex = std::rand() % grassTiles.size();
		glm::vec2 randomTile = grassTiles[randomIndex];
		glm::ivec2 grassTileIndex = Map2D::GetInstance()->PosToTilePos(randomTile);

		tilemap[grassTileIndex.y][grassTileIndex.x]->tileID = 5;
		modifiedTiles.insert({ grassTileIndex, 2 });
	}

	Init();
	Map2D::GetInstance()->GenerateNodes();
}

void MazeMap::WaterEvent()
{
	std::vector<glm::vec2> waterTiles = GetTilesWithID(4);
	for (int i = 0; i < waterTiles.size(); i++)
	{
		glm::ivec2 waterTileIndex = Map2D::GetInstance()->PosToTilePos(waterTiles[i]);
		if (waterTileIndex.y > 0 && tilemap[waterTileIndex.y - 1][waterTileIndex.x]->Passability == 0)
		{
			modifiedTiles.insert({ glm::ivec2(waterTileIndex.x, waterTileIndex.y - 1), tilemap[waterTileIndex.y - 1][waterTileIndex.x]->tileID });
			tilemap[waterTileIndex.y - 1][waterTileIndex.x]->tileID = 4;
		}
		if (waterTileIndex.y < MapSizeY - 1 && tilemap[waterTileIndex.y + 1][waterTileIndex.x]->Passability == 0)
		{
			modifiedTiles.insert({ glm::ivec2(waterTileIndex.x, waterTileIndex.y + 1), tilemap[waterTileIndex.y + 1][waterTileIndex.x]->tileID });
			tilemap[waterTileIndex.y + 1][waterTileIndex.x]->tileID = 4;
		}
		if (waterTileIndex.x > 0 && tilemap[waterTileIndex.y][waterTileIndex.x - 1]->Passability == 0)
		{
			modifiedTiles.insert({ glm::ivec2(waterTileIndex.x - 1, waterTileIndex.y), tilemap[waterTileIndex.y][waterTileIndex.x - 1]->tileID });
			tilemap[waterTileIndex.y][waterTileIndex.x - 1]->tileID = 4;
		}
		if (waterTileIndex.x < MapSizeX - 1 && tilemap[waterTileIndex.y][waterTileIndex.x + 1]->Passability == 0)
		{
			modifiedTiles.insert({ glm::ivec2(waterTileIndex.x + 1, waterTileIndex.y), tilemap[waterTileIndex.y][waterTileIndex.x + 1]->tileID });
			tilemap[waterTileIndex.y][waterTileIndex.x + 1]->tileID = 4;
		}
	}

	Init();
	Map2D::GetInstance()->GenerateNodes();
}

void MazeMap::Drought()
{
	std::vector<glm::vec2> waterTiles = GetTilesWithID(4);
	for (int i = 0; i < waterTiles.size(); i++)
	{
		glm::ivec2 waterTileIndex = Map2D::GetInstance()->PosToTilePos(waterTiles[i]);
		modifiedTiles.insert({ glm::ivec2(waterTileIndex.x, waterTileIndex.y), tilemap[waterTileIndex.y][waterTileIndex.x]->tileID });

		tilemap[waterTileIndex.y][waterTileIndex.x]->tileID = 3;
	}

	Init();
	Map2D::GetInstance()->GenerateNodes();
}

void MazeMap::Floodfill(glm::ivec2 startIndex, std::vector<glm::ivec2>& reachableTiles)
{
	if (startIndex.x < 0 || startIndex.x >= MapSizeX ||
		startIndex.y < 0 || startIndex.y >= MapSizeY || 
		tilemap[startIndex.y][startIndex.x]->Passability == 0) {
		return;
	}

	auto it = std::find(reachableTiles.begin(), reachableTiles.end(), startIndex);
	if (it != reachableTiles.end()) { return; }

	reachableTiles.push_back(startIndex);
	
	Floodfill(glm::ivec2(startIndex.x + 1, startIndex.y), reachableTiles);
	Floodfill(glm::ivec2(startIndex.x - 1, startIndex.y), reachableTiles);
	Floodfill(glm::ivec2(startIndex.x, startIndex.y + 1), reachableTiles);
	Floodfill(glm::ivec2(startIndex.x, startIndex.y - 1), reachableTiles);
}

MazeMap::MazeMap(unsigned int MapX, unsigned int MapY, glm::vec2 TileSize, PerlinNoiseConfig* Config1D, PerlinNoiseConfig* Config2D) : BaseLevel(MapX, MapY, TileSize, Config1D, Config2D)
{
	this->MapSizeX = MapX;
	this->MapSizeY = MapY;
	this->tileSize = TileSize;


	std::vector<std::vector<int>> noise1D;
	std::vector<std::vector<int>> noise2D;
	

	if (Config2D != nullptr) {
		noise2D = Generate2DPerlinMap(Config2D);
		this->Config2D = Config2D;
	}

	GeneratePerlinMap(noise1D, noise2D);

}
