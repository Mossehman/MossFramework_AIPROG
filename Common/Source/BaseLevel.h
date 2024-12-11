#pragma once
#include "Tile.h"

#include <vector>
#include <map>

#define AIR_ID 0 //define our Air ID for skipping on rendering and collision checks

struct PerlinNoiseConfig
{
	int HeightOffset;
	int Octaves;
	float ScalingBias;

	PerlinNoiseConfig(int Octaves, float ScalingBias, int HeightOffset = 0)
	{
		this->Octaves = Octaves;
		this->ScalingBias = ScalingBias;
		this->HeightOffset = HeightOffset;
	}
};

/// <summary>
/// Abstract class, levels should derive and overwrite from this class
/// </summary>
class BaseLevel
{
protected:
	std::vector<std::vector<Tile*>> tilemap;
	unsigned int MapSizeX;
	unsigned int MapSizeY;

	std::map<int, int> textureMap;
	std::map<int, bool> passabilityMap;

	glm::vec2 tileSize;
	Mesh* tileMesh;

	/// <summary>
	/// To generate the tile data for each level, left abstract as different IDs may have different tile data
	/// </summary>
	virtual void InitTiles() = 0;

	/// <summary>
	/// Binds a texture file to a tileID for rendering
	/// </summary>
	/// <param name="ID"> - The tileID we want to bind the texture to</param>
	/// <param name="textureFile"> - The file path of the texture file</param>
	void BindTexture(unsigned int ID, const char* textureFile);

	/// <summary>
	/// Binds a boolean variable to a tileID for Pathfinding and Collision
	/// </summary>
	/// <param name="ID"> - The tileID we want to modify the passability of</param>
	/// <param name="isPassable"> - Determines if the tile can be passed or not</param>
	void BindPassability(unsigned int ID, int isPassable);

	/// <summary>
	/// Generates the raw noise values for 1-Dimensional noise
	/// </summary>
	/// <param name="width"> - The horizontal length of the noise</param>
	/// <param name="seed"> - The random seed for generating the noise</param>
	/// <param name="octaves"> - The noise octaves for perlin noise tuning</param>
	/// <param name="bias"> - The bumpiness/height difference in the noise</param>
	/// <param name="Output"> - The output floating point values</param>
	void PerlinNoise1D(int width, float* seed, int octaves, float bias, float* Output);

	/// <summary>
	/// Generates the raw noise values for 2-Dimensional noise
	/// </summary>
	/// <param name="width"> - The horizontal length of the noise</param>
	/// <param name="height"> - The vertical height of the noise</param>
	/// <param name="seed"> - The random seed for generating the noise</param>
	/// <param name="octaves"> - The noise octaves for perlin noise tuning</param>
	/// <param name="bias"> - The bumpiness/height difference in the noise</param>
	/// <param name="Output"> - The output floating point values</param>
	void PerlinNoise2D(int width, int height, float* seed, int octaves, float bias, float* Output);

	/// <summary>
	/// Generates the tileIDs for the 1-Dimensional tilemap, left abstract for specification in derived classes
	/// </summary>
	/// <param name="width"> - The width of our map</param>
	/// <param name="height"> - The height of our map</param>
	/// <param name="heightOffset"> - The ground offset (higher value = lower max height)</param>
	/// <param name="Input"> - The noise values for generating the tilemap</param>
	/// <param name="noiseMap"> - The output tilemap ID values</param>
	virtual void NoiseToTiles1D(int width, int height, int heightOffset, const float* Input, std::vector<std::vector<int>>& noiseMap) {}

	/// <summary>
	/// Generates the tileIDs for the 2-Dimensional tilemap, left abstract for specification in derived classes
	/// </summary>
	/// <param name="width"> - The width of our map</param>
	/// <param name="height"> - The height of our map</param>
	/// <param name="Input"> - The noise values for generating the tilemap</param>
	/// <param name="noiseMap"> - The output tilemap ID values</param>
	virtual void NoiseToTiles2D(int width, int height, const float* Input, std::vector<std::vector<int>>& noiseMap) {}

	/// <summary>
	/// Generates a random seed for our noise values
	/// </summary>
	/// <param name="seed"> - The seed value (output)</param>
	/// <param name="count"> - The number of seed values</param>
	void GenerateSeed(std::vector<float>& seed, unsigned int count, bool overwrite = true);

	/// <summary>
	/// Generates the tilemap based on 1D and 2D perlin noise (modifable in derived classes)
	/// </summary>
	virtual void GeneratePerlinMap(std::vector<std::vector<int>> firstMap, std::vector<std::vector<int>> otherMap);

	std::vector<std::vector<int>> Generate1DPerlinMap(PerlinNoiseConfig* noiseParameters);	
	std::vector<std::vector<int>> Generate2DPerlinMap(PerlinNoiseConfig* noiseParameters);
	
	void WriteMapFromCSV(const std::string& filename);
	void SaveMapToCSV(const std::string& filename);

public:
	BaseLevel(unsigned int MapX, unsigned int MapY, glm::vec2 TileSize, const std::string& filename);
	BaseLevel(unsigned int MapX, unsigned int MapY, glm::vec2 TileSize, PerlinNoiseConfig* Config1D, PerlinNoiseConfig* Config2D);

	virtual void Init();
	virtual void Update(float dt);
	virtual void Render(glm::ivec2 CameraPos, glm::ivec2 RenderDistance);


	void SetTileSize(glm::vec2 tileSize);
	glm::vec2 GetTileSize(void);

	void SetTileMesh(Mesh* mesh);
	Mesh* GetTileMesh(void);

	int GetMapX(void);
	int GetMapY(void);

	std::vector<std::vector<Tile*>>& GetTilemap(void);

	std::vector<glm::vec2> GetTilesWithID(int IDToFind);

	void ClearMap();

};

