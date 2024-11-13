#include "BaseLevel.h"
#include "ImageLoader.h"
#include <fstream>
#include <sstream>

void BaseLevel::BindTexture(unsigned int ID, const char* textureFile)
{
	unsigned int TextureID = ImageLoader::GetInstance()->LoadTextureGetID(textureFile, true);

	if (TextureID == 0)
	{
		std::cout << "Unable to load texture: " << textureFile << std::endl;
		return;
	}

	textureMap.insert(std::pair<int, int>(ID, TextureID));
}

void BaseLevel::BindPassability(unsigned int ID, bool isPassable)
{
	passabilityMap.insert(std::pair<int, int>(ID, isPassable));
}

void BaseLevel::PerlinNoise1D(int width, float* seed, int octaves, float bias, float* Output)
{
	for (int x = 0; x < width; x++)
	{
		float Noise = 0.f;
		float Scale = 1.0f;
		float Accumulation = 0.f;

		for (int o = 0; o < octaves; o++)
		{
			int Pitch = width >> o;
			if (Pitch == 0) { break; }

			int FirstSample = (x / Pitch) * Pitch;
			int SecondSample = (FirstSample + Pitch) % width;

			float Blend = (float)(x - FirstSample) / (float)Pitch;
			float Sample = (1.0f - Blend) * seed[FirstSample] + Blend * seed[SecondSample];
			Noise += Sample * Scale;

			Accumulation += Scale;
			Scale = Scale / bias;
		}

		Output[x] = Noise / Accumulation;
	}
}

void BaseLevel::PerlinNoise2D(int width, int height, float* seed, int octaves, float bias, float* Output)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			float Noise = 0.f;
			float Scale = 1.0f;
			float Accumulation = 0.f;

			for (int o = 0; o < octaves; o++)
			{
				int Pitch = width >> o;
				if (Pitch == 0) { break; }

				int FirstSampleX = (x / Pitch) * Pitch;
				int FirstSampleY = (y / Pitch) * Pitch;

				int SecondSampleX = (FirstSampleX + Pitch) % width;
				int SecondSampleY = (FirstSampleY + Pitch) % height;

				float BlendX = (float)(x - FirstSampleX) / (float)Pitch;
				float BlendY = (float)(y - FirstSampleY) / (float)Pitch;

				float SampleT = (1.0f - BlendX) * seed[FirstSampleY * width + FirstSampleX] + BlendX * seed[FirstSampleY * width + SecondSampleX];
				float SampleB = (1.0f - BlendX) * seed[SecondSampleY * width + FirstSampleX] + BlendX * seed[SecondSampleY * width + SecondSampleX];

				Noise += (BlendY * (SampleB - SampleT) + SampleT) * Scale;
				Accumulation += Scale;
				Scale = Scale / bias;
			}
			Output[y * width + x] = Noise / Accumulation;
		}
	}
}



void BaseLevel::GenerateSeed(std::vector<float>& seed, unsigned int count, bool overwrite)
{
	//ensure you call srand before this
	if (overwrite) { seed.clear(); }

	for (int i = 0; i < count; i++)
	{
		seed.push_back(static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
	}
}

void BaseLevel::GeneratePerlinMap(std::vector<std::vector<int>> firstMap, std::vector<std::vector<int>> otherMap)
{
	std::vector<std::vector<int>> mapToUse;
	tilemap.resize(MapSizeY);
	mapToUse.resize(MapSizeY);

	if (firstMap.size() <= 0 && otherMap.size() <= 0)
	{
		for (int y = 0; y < MapSizeY; y++)
		{
			for (int x = 0; x < MapSizeX; x++)
			{
				Tile* newTile = new Tile(AIR_ID);
				tilemap[y].push_back(newTile);
			}
		}
	}

	if (firstMap.size() <= 0)
	{
		mapToUse = otherMap;
	}
	else if (otherMap.size() <= 0)
	{
		mapToUse = firstMap;
	}
	else
	{
		for (int y = 0; y < MapSizeY; y++)
		{
			for (int x = 0; x < MapSizeX; x++)
			{
				if (firstMap[y][x] == AIR_ID) 
				{ 
					mapToUse[y][x] = otherMap[y][x]; 
					continue;
				}
				
				mapToUse[y][x] = firstMap[y][x];

			}
		}
	}

	for (int y = 0; y < MapSizeY; y++)
	{
		for (int x = 0; x < MapSizeX; x++)
		{
			Tile* newTile = new Tile(mapToUse[y][x]);
			tilemap[y].push_back(newTile);
		}
	}
}

std::vector<std::vector<int>> BaseLevel::Generate1DPerlinMap(PerlinNoiseConfig* noiseParameters)
{
	std::vector<float>SeedVal1D;
	std::vector<float>Output1D;
	
	Output1D.resize(MapSizeX); //define a size for this vector

	GenerateSeed(SeedVal1D, MapSizeX, true);

	PerlinNoise1D(MapSizeX, SeedVal1D.data(), noiseParameters->Octaves, noiseParameters->ScalingBias, Output1D.data());

	std::vector<std::vector<int>> tileMap1D(MapSizeY, std::vector<int>(MapSizeX, 0));
	NoiseToTiles1D(MapSizeX, MapSizeY, noiseParameters->HeightOffset, Output1D.data(), tileMap1D);

	return tileMap1D;
	
}

std::vector<std::vector<int>> BaseLevel::Generate2DPerlinMap(PerlinNoiseConfig* noiseParameters)
{
	std::vector<float>SeedVal2D;
	std::vector<float>Output2D;

	Output2D.resize(MapSizeX * MapSizeY); //define a size for this vector

	GenerateSeed(SeedVal2D, MapSizeX * MapSizeY, true);

	PerlinNoise2D(MapSizeX, MapSizeY, SeedVal2D.data(), noiseParameters->Octaves, noiseParameters->ScalingBias, Output2D.data());

	std::vector<std::vector<int>> tileMap2D(MapSizeY, std::vector<int>(MapSizeX, 0));
	NoiseToTiles2D(MapSizeX, MapSizeY, Output2D.data(), tileMap2D);

	return tileMap2D;
}

void BaseLevel::WriteMapFromCSV(const std::string& filename)
{
	std::ifstream file(filename);
	std::string line;

	if (!file.is_open())
	{
		std::cout << "Unable to open file: " << filename << std::endl;
		return;
	}

	ClearMap();

	tilemap.resize(MapSizeY);

	for (int y = 0; y < MapSizeY; y++)
	{
		std::getline(file, line);
		std::stringstream linestream(line);
		std::string cell;

		// Resize the row to avoid push_back
		tilemap[y].resize(MapSizeX);

		for (int x = 0; x < MapSizeX; x++)
		{
			std::getline(linestream, cell, ',');
			if (cell.empty()) { cell = std::to_string(AIR_ID); } //default to AIR_ID if cell is empty
			tilemap[y][x] = new Tile(std::stoi(cell));
		}
	}

	file.close();
}


void BaseLevel::SaveMapToCSV(const std::string& filename)
{
	std::ofstream file(filename);

	for (int y = 0; y < tilemap.size(); y++)
	{
		for (int x = 0; x < tilemap[y].size(); x++)
		{
			file << tilemap[y][x]->tileID;
			if (x < tilemap[y].size() - 1)
			{
				file << ",";
			}
		}
		file << "\n";
	}
	file.close();

}
 
BaseLevel::BaseLevel(unsigned int MapX, unsigned int MapY, glm::vec2 TileSize, const std::string& filename)
{
	this->MapSizeX = MapX;
	this->MapSizeY = MapY;
	this->tileSize = TileSize;

	this->WriteMapFromCSV(filename);
}

BaseLevel::BaseLevel(unsigned int MapX, unsigned int MapY, glm::vec2 TileSize, PerlinNoiseConfig* Config1D, PerlinNoiseConfig* Config2D)
{
	this->MapSizeX = MapX;
	this->MapSizeY = MapY;
	this->tileSize = TileSize;


	std::vector<std::vector<int>> noise1D;
	std::vector<std::vector<int>> noise2D;

	if (Config1D != nullptr) {
		noise1D = Generate1DPerlinMap(Config1D);
	}

	if (Config2D != nullptr) {
		noise2D = Generate2DPerlinMap(Config2D);
	}

	GeneratePerlinMap(noise1D, noise2D);
	
}

void BaseLevel::Init()
{
	InitTiles();
	BindPassability(0, true);

	for (int y = 0; y < MapSizeY; y++)
	{
		for (int x = 0; x < MapSizeX; x++)
		{
			std::cout << x << std::endl;

			if (passabilityMap.size() > 0)
			{
				if (passabilityMap.count(tilemap[y][x]->tileID) > 0)
				{
					tilemap[y][x]->isPassable = passabilityMap.at(tilemap[y][x]->tileID);
				}
			}
		
			tilemap[y][x]->setPosition(glm::vec2(x * tileSize.x, -y * tileSize.y));
			tilemap[y][x]->setScale(tileSize);
			
			tilemap[y][x]->SetMesh(tileMesh);
			if (textureMap.count(tilemap[y][x]->tileID) > 0)
			{
				tilemap[y][x]->textureID = textureMap.at(tilemap[y][x]->tileID);
			}
		}
	}
}

void BaseLevel::Update(float dt)
{
}

void BaseLevel::Render(glm::ivec2 CameraTile, glm::ivec2 RenderDistance)
{
	//iterate through all the tiles on the tilemap for rendering
	for (int y = std::max(CameraTile.y - RenderDistance.y, 0); y < std::min(CameraTile.y + RenderDistance.y, (int)MapSizeY); y++)
	{
		for (int x = std::max(CameraTile.y - RenderDistance.y, 0); x < std::min(CameraTile.x + RenderDistance.x, (int)MapSizeX); x++)
		{
			if (tilemap[y][x]->tileID == AIR_ID) { continue; } //do not render air tiles
			tilemap[y][x]->Render();
		}
	}
}

void BaseLevel::SetTileSize(glm::vec2 tileSize)
{
	this->tileSize = tileSize;
}

glm::vec2 BaseLevel::GetTileSize(void)
{
	return tileSize;
}

void BaseLevel::SetTileMesh(Mesh* mesh)
{
	if (mesh == nullptr) { return; }
	this->tileMesh = mesh;
}

Mesh* BaseLevel::GetTileMesh(void)
{
	return this->tileMesh;
}

int BaseLevel::GetMapX(void)
{
	return this->MapSizeX;
}

int BaseLevel::GetMapY(void)
{
	return this->MapSizeY;
}

std::vector<std::vector<Tile*>>& BaseLevel::GetTilemap(void)
{
	return this->tilemap;
}

void BaseLevel::ClearMap()
{
	for (int y = 0; y < tilemap.size(); y++)
	{
		for (int x = 0; x < tilemap[y].size(); y++)
		{
			if (!tilemap[y][x]) { continue; }
			delete tilemap[y][x]; 
		}
		tilemap[y].clear();
	}

	tilemap.clear();
}
