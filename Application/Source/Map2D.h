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
//#include "LevelIDs.h"


struct Tile : public GameObject
{
	unsigned int tileID;
	bool isPassable = false;
	bool isDangerous = false;

	Tile(int tileIndex, glm::vec2 pos = glm::vec2(0, 0), glm::vec2 rot = glm::vec2(0, 0), glm::vec2 scl = glm::vec2(0, 0))
	{
		tileID = tileIndex;
		position = pos;
		rotation = rot;
		scale = scl;
		isPassable = false;
		isDangerous = false;
	}
};

//we use enums for ease of tracking our levels, to avoid magic numbers and to avoid accidentally generating a new level

class Map2D : public SingletonTemplate<Map2D>
{
	friend SingletonTemplate<Map2D>;

public:

	std::vector<glm::vec2> tilesFound;

	std::vector<int> updatedTileYindex;
	std::vector<int> updatedTileXindex;
	bool mapUpdate = false;

	//defines our rows for each level in our map
	std::vector<unsigned int> mapSizeX;


	//defines our columns for each level in our map
	std::vector<unsigned int> mapSizeY;

	//3d vector for storing our tiles
	std::vector<std::vector<std::vector<Tile*>>> tileMaps;

	void SetTileMesh(Mesh* mesh);
	Mesh* GetTileMesh(void);

	/*!
	@brief Sets the width and height of 1 tile on the tilemap

	\param newTileSize - The height and width of 1 tile, ensure both values are above 0
	*/
	void SetTileSize(glm::vec2 newTileSize);
	glm::vec2 GetTileSize(void);
	
	/*!
	@brief Generates floating point values via 1D perlin noise
	
	\param nCount - The width of the 1D perlin noise
	\param fSeed - The seed for each tile of the perlin noise to sample from
	\param nOctaves - The level of detail/smoothing value of the perlin noise
	\param fBias - The intensity of the perlin noise (higher -> less intense)
	\param fOutput - The float array for the perlin noise values to be output to
	
	*/
	void PerlinNoise1D(int nCount, float* fSeed, int nOctaves, float fBias, float* fOutput);

	/*!
	@brief Generates floating point values via 2D perlin noise
	
	\param nWidth - The width of the 2D perlin noise
	\param nHeight - The height of the 2D perlin noise
	\param fSeed - The seed for each tile of the perlin noise to sample from
	\param nOctaves - The level of detail/smoothing value of the perlin noise
	\param fBias - The intensity of the perlin noise (higher -> less intense)
	\param fOutput - The float array for the perlin noise values to be output to
	
	*/
	void PerlinNoise2D(int nWidth, int nHeight, float* fSeed, int nOctaves, float fBias, float* fOutput);

	/*!
	@brief Translates the floating point output from perlinNoise1D() into integer tile Indexes, typically used for ground level, left blank for overwriting with specific parameters

	\param width - The width of the generated tileMap
	\param height - The height of the generated tileMap
	\param heightOffset - How far down the generated tileMap is pushed
	\param heightOffset - the output value of perlinNoise1D() to read from
	\param noiseTileMap - the tilemap to write the tile indexes to

	*/
	virtual void GenerateTileMap1D(int width, int height, int heightOffset, const float* fOutput, std::vector<std::vector<int>>& noiseTileMap) {}

	/*!
	@brief Translates the floating point output from perlinNoise2D() into integer tile Indexes, left blank for overwriting with specific parameters
	
	\param width - The width of the generated tileMap
	\param height - The height of the generated tileMap
	\param heightOffset - the output value of perlinNoise1D() to read from
	\param noiseTileMap - the tilemap to write the tile indexes to
	\param minAirRange - ignores tiles above this value and converts them to air tiles
	\param maxAirRange - ignores tiles below this value and converts them to air tiles
	
	*/
	virtual void GenerateTileMap2D(int width, int height, const float* fOutput, std::vector<std::vector<int>>& noiseTileMap) {}

	/*!
	@brief Generates a random seed value for each tile
	
	\param seed - the vector of seed inputs for each tile to sample from
	\param width - how far the seed generation should go on the X axis
	
	*/
	void GenerateRandomSeed(std::vector<float> seed, unsigned int width);

	virtual void GeneratePerlinMap(int levelID);

	/*!
	@brief Blends the 1D and 2D tilemap and writes it to the map level at the specified level ID

	\param tileMap1D - the 1D tilemap indexes
	\param tileMap2D - the 2D tilemap indexes
	\param levelID - the map level to write the tilemap to
	*/
	virtual void WriteNoiseMap(const std::vector<std::vector<int>>& tileMap1D, const std::vector<std::vector<int>>& tileMap2D, int levelID) {}
	void WriteMapFromCSV(const std::string& filename, int levelID);
	void SaveMapToCSV(const std::string filename, int levelID);

	void ChangeLevel(int newLevel);
	int GetCurrentLevel(void);

	int GetMapInfo(unsigned int row, unsigned int column, int levelToCheck = -1);
	bool GetMapPassable(unsigned int row, unsigned int column, int levelToCheck = -1);
	bool GetMapDangeous(unsigned int row, unsigned int column, int levelToCheck = -1);
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
	
	virtual void Init(int startLevel);
	virtual void Update();
	void Render(glm::vec2 cameraPos, glm::ivec2 renderDistance);

	void PrintMapData(int level);


protected:
	std::map<int, int> textureMaps;
	std::map<int, bool> passableTiles;
	std::map<int, bool> dangerousTiles;

	int currentLevel;

	const int maxLevels;

	void SetTextureToID(unsigned int ID, const char* textureName); 
	void SetPassableTile(unsigned int ID, bool isPassable); 
	void SetDangerousTile(unsigned int ID, bool isDangerous); 

	virtual void BindTextures() {}

	/*!
	@brief Wrapper function for specifying Noise Map generation per-project, left blank for overwriting with specific parameters
	*/
	virtual void OnWriteNoise(const std::vector<std::vector<int>>& tileMap1D, const std::vector<std::vector<int>>& tileMap2D, int levelID, int y, int x) {}

	glm::vec2 tileSize; //X and Y dimensions of each tile

	Mesh* tileMesh;

	std::vector<int> currentMapSizeX;
	std::vector<int> currentMapSizeY;

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
	
	Map2D(const int MaxLevels = 1);
	~Map2D(void);
};

