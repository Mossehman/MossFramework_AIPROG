#pragma once
#include "DesignPatterns/SingletonTemplate.h"
#include "GameObject.h"
#include "LoadTGA.h"
#include "Mesh.h"
#include "AStar.h"
#include "Tile.h"

#include "BaseLevel.h"

#pragma warning(push)
#pragma warning(disable:6385)

#include <glm\glm.hpp>

#include <map>
#include <vector>
#include <string>


//#include "LevelIDs.h"

//we use enums for ease of tracking our levels, to avoid magic numbers and to avoid accidentally generating a new level

class Map2D : public SingletonTemplate<Map2D>
{
	friend SingletonTemplate<Map2D>;

public:

	std::vector<glm::vec2> tilesFound;

	std::vector<int> updatedTileYindex;
	std::vector<int> updatedTileXindex;
	bool mapUpdate = false;

	//3d vector for storing our tiles
	std::map<int, BaseLevel*> Levels;

	void ChangeLevel(int newLevel);
	int GetCurrentLevel(void);

	int GetMapInfo(unsigned int row, unsigned int column, int levelToCheck = -1);
	bool GetMapPassable(unsigned int row, unsigned int column, int levelToCheck = -1);
	void SetMapInfo(unsigned int row, unsigned int column, unsigned int newTileID, int levelToCheck = -1);

	/*!
	@brief Converts world coordinates into tile coordinates (columns and rows)

	\param position - the coordinate values to convert into tile coordinates
	\param mode - to round down, use automatic truncation or round up (-1, 0 and 1)

	*/
	glm::ivec2 PosToTilePos(glm::vec2 position, int mode = 0);

	void FindTileID(int tileID, int level = -1);
	
	/*!
	@brief Converts tile coordinates to world coordinates

	\param tilePosition - the tile column and row to convert to world coordinates
	*/
	glm::vec2 TileToPos(glm::vec2 tilePosition);
	
	void Init(int startLevel);
	void Update(float dt);
	void Render(glm::vec2 cameraPos, glm::ivec2 renderDistance);

	void PrintMapData(int level);

	virtual void GenerateNodes();
	void RenderNodes();
	AStar* GetAStar();

	void AddLevel(int levelID, BaseLevel* newLevel, Mesh* tileMesh = MeshBuilder::GenerateQuad("Tile", Color(1, 1, 1)));
	BaseLevel* GetLevel(int levelID = -1);

private:
	int currentLevel;
	int maxLevels;
	AStar* AStarNodes;
	~Map2D();
};

