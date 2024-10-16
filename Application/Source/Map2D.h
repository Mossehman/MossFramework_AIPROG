#pragma once
#include "DesignPatterns/SingletonTemplate.h"
#include "GameObject.h"
#include "LoadTGA.h"
#include "Mesh.h"

#pragma warning(push)
#pragma warning(disable:6385)

#include <glm\glm.hpp>

#include <map>
#include <vector>
#include <string>
#include "pathNode.h"
#include "LevelIDs.h"


struct tile : public GameObject
{
	unsigned int tileID;
	bool isPassable = false;
	bool isDangerous = false;

	tile(int tileIndex, glm::vec2 pos = glm::vec2(0, 0), glm::vec2 rot = glm::vec2(0, 0), glm::vec2 scl = glm::vec2(0, 0))
	{
		tileID = tileIndex;
		position = pos;
		rotation = rot;
		scale = scl;
		isPassable = false;
		isDangerous = false;
	}
};

//we use enums for ease of tracking our levels, to avoid magic numbers and to avoid accidentally generating a new leve

class Map2D : public SingletonTemplate<Map2D>
{
	friend SingletonTemplate<Map2D>;

public:

	std::vector<glm::vec2> tilesFound;

	std::vector<int> updatedTileYindex;
	std::vector<int> updatedTileXindex;
	bool mapUpdate = false;

	//defines our rows for each level in our map
	unsigned int mapSizeX[NUM_LEVELS];

	//defines our columns for each level in our map
	unsigned int mapSizeY[NUM_LEVELS];

	//3d vector for storing our tiles
	std::vector<std::vector<tile*>> tileMaps[NUM_LEVELS];

	void setTileMesh(Mesh* mesh);
	Mesh* getTileMesh(void);

	/*!
	@brief Sets the width and height of 1 tile on the tilemap

	\param newTileSize - The height and width of 1 tile, ensure both values are above 0
	*/
	void setTileSize(glm::vec2 newTileSize);
	glm::vec2 getTileSize(void);
	
	/*!
	@brief Generates floating point values via 1D perlin noise
	
	\param nCount - The width of the 1D perlin noise
	\param fSeed - The seed for each tile of the perlin noise to sample from
	\param nOctaves - The level of detail/smoothing value of the perlin noise
	\param fBias - The intensity of the perlin noise (higher -> less intense)
	\param fOutput - The float array for the perlin noise values to be output to
	
	*/
	void perlinNoise1D(int nCount, float* fSeed, int nOctaves, float fBias, float* fOutput);

	/*!
	@brief Generates floating point values via 2D perlin noise
	
	\param nWidth - The width of the 2D perlin noise
	\param nHeight - The height of the 2D perlin noise
	\param fSeed - The seed for each tile of the perlin noise to sample from
	\param nOctaves - The level of detail/smoothing value of the perlin noise
	\param fBias - The intensity of the perlin noise (higher -> less intense)
	\param fOutput - The float array for the perlin noise values to be output to
	
	*/
	void perlinNoise2D(int nWidth, int nHeight, float* fSeed, int nOctaves, float fBias, float* fOutput);

	/*!
	@brief Translates the floating point output from perlinNoise1D() into integer tile Indexes, typically used for ground level

	\param width - The width of the generated tileMap
	\param height - The height of the generated tileMap
	\param heightOffset - How far down the generated tileMap is pushed
	\param heightOffset - the output value of perlinNoise1D() to read from
	\param noiseTileMap - the tilemap to write the tile indexes to

	*/
	void generateTileMap1D(int width, int height, int heightOffset, const float* fOutput, std::vector<std::vector<int>>& noiseTileMap);

	/*!
	@brief Translates the floating point output from perlinNoise2D() into integer tile Indexes
	
	\param width - The width of the generated tileMap
	\param height - The height of the generated tileMap
	\param heightOffset - the output value of perlinNoise1D() to read from
	\param noiseTileMap - the tilemap to write the tile indexes to
	\param minAirRange - ignores tiles above this value and converts them to air tiles
	\param maxAirRange - ignores tiles below this value and converts them to air tiles
	
	*/
	void generateTileMap2D(int width, int height, const float* fOutput, std::vector<std::vector<int>>& noiseTileMap, int minAirRange = 4, int maxAirRange = 6);

	/*!
	@brief Generates a random seed value for each tile
	
	\param seed - the vector of seed inputs for each tile to sample from
	\param width - how far the seed generation should go on the X axis
	
	*/
	void generateRandomSeed(std::vector<float> seed, unsigned int width);

	void generatePerlinMap(LevelIDs levelID);

	/*!
	@brief Blends the 1D and 2D tilemap and writes it to the map level at the specified level ID

	\param tileMap1D - the 1D tilemap indexes
	\param tileMap2D - the 2D tilemap indexes
	\param levelID - the map level to write the tilemap to
	*/
	void writeNoiseMap(const std::vector<std::vector<int>>& tileMap1D, const std::vector<std::vector<int>>& tileMap2D, LevelIDs levelID);
	void writeMapFromCSV(const std::string& filename, LevelIDs levelID);
	void saveMapToCSV(const std::string filename, LevelIDs levelID);

	void changeLevel(LevelIDs newLevel);
	LevelIDs getCurrentLevel(void);

	int getMapInfo(unsigned int row, unsigned int column, LevelIDs levelToCheck = LevelIDs::NUM_LEVELS);
	bool getMapPassable(unsigned int row, unsigned int column, LevelIDs levelToCheck = LevelIDs::NUM_LEVELS);
	bool getMapDangeous(unsigned int row, unsigned int column, LevelIDs levelToCheck = LevelIDs::NUM_LEVELS);
	void setMapInfo(unsigned int row, unsigned int column, unsigned int newTileID, LevelIDs levelToCheck = LevelIDs::NUM_LEVELS);

	/*!
	@brief Converts world coordinates into tile coordinates (columns and rows)

	\param position - the coordinate values to convert into tile coordinates
	\param mode - to round down, use automatic truncation or round up (-1, 0 and 1)

	*/
	glm::vec2 posToTilePos(glm::vec2 position, int mode = 0);

	void findTileID(int tileID, LevelIDs level = NUM_LEVELS);
	
	/*!
	@brief Converts tile coordinates to world coordinates

	\param tilePosition - the tile column and row to convert to world coordinates
	*/
	glm::vec2 tileToPos(glm::vec2 tilePosition);
	
	void Init(LevelIDs startLevel);
	void Update();
	void Render(glm::vec2 cameraPos, glm::vec2 renderDistance);

	void printMapData(LevelIDs level);


protected:
	std::map<int, int> textureMaps;
	std::map<int, bool> passableTiles;
	std::map<int, bool> dangerousTiles;

	LevelIDs currentLevel;


	void setTextureToID(unsigned int ID, const char* textureName);
	void setPassableTile(unsigned int ID, bool isPassable);
	void setDangerousTile(unsigned int ID, bool isDangerous);

	glm::vec2 tileSize; //X and Y dimensions of each tile

	Mesh* tileMesh;

	int currentMapSizeX[NUM_LEVELS];
	int currentMapSizeY[NUM_LEVELS];

	//For perlin noise (for more info, read the briefs on the perlin noise functions)

	int heightOffset = 110; //height offset for 2D perlin noise
	int octaves1D = 5; //level of detail for 1D perlin noise
	int octaves2D = 5; //level of detail for 2D perlin noise
	float scalingBias1D = 1.2f; //intensity value for 1D perlin noise
	float scalingBias2D = 0.6f; //intensity value for 2D perlin noise

	std::vector<float> fSeed1D; //seed for 1D perlin noise
	std::vector<float> fSeed2D; //seed for 2D perlin noise

	std::vector<float> fOutput1D; //the output 1D perlin noise
	std::vector<float> fOutput2D; //the output 2D perlin noise

	Map2D(void);
	~Map2D(void);
};

