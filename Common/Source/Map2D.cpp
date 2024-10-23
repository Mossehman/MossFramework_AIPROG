#include "Map2D.h"
#include <fstream>
#include <sstream>
#include "ImageLoader.h"


#define AIR_ID 0
#define STONE_ID 1
#define DIRT_ID 2
#define GRASS_ID 3


int Map2D::GetMapInfo(unsigned int row, unsigned int column, int levelToCheck)
{
	if (levelToCheck == -1) //if we are not checking a specific level
	{
		levelToCheck = currentLevel; //check the currently active level
	}

    if (column >= tileMaps[levelToCheck].size() || row >= tileMaps[levelToCheck][column].size()) //check if tile or row is out of range
    {
        return 0;
    }

    return tileMaps[levelToCheck][row][column]->tileID;
}

bool Map2D::GetMapPassable(unsigned int row, unsigned int column, int levelToCheck)
{
    if (levelToCheck == -1) //if we are not checking a specific level
    {
        levelToCheck = currentLevel; //check the currently active level
    }

    if (column >= tileMaps[levelToCheck].size() || row >= tileMaps[levelToCheck][column].size()) //check if tile or row is out of range
    {
        return false;
    }

    return tileMaps[levelToCheck][row][column]->isPassable;
}


bool Map2D::GetMapDangeous(unsigned int row, unsigned int column, int levelToCheck)
{
    if (levelToCheck == -1) //if we are not checking a specific level
    {
        levelToCheck = currentLevel; //check the currently active level
    }

    if (column >= tileMaps[levelToCheck].size() || row >= tileMaps[levelToCheck][column].size()) //check if tile or row is out of range
    {
        return false;
    }

    return tileMaps[levelToCheck][row][column]->isDangerous;
}


void Map2D::SetMapInfo(unsigned int row, unsigned int column, unsigned int newTileID, int levelToCheck)
{
    if (levelToCheck == -1) //if we are not checking a specific level
    {
        levelToCheck = currentLevel; //check the currently active level
    }

    if (column > tileMaps[levelToCheck].size() || row > tileMaps[levelToCheck][column].size()) //check if tile or row is out of range
    {
        return;
    }
    if (tileMaps[levelToCheck][row][column]->tileID == newTileID) { return; } //tile to replace is the same as the one on the current map, do not replace


    tileMaps[levelToCheck][row][column]->tileID = newTileID;

    //cache the modified row and column values to better optimise other systems (clear this at the end of every frame)
    updatedTileXindex.push_back(column);
    updatedTileYindex.push_back(row);

    mapUpdate = true;
}

glm::vec2 Map2D::PosToTilePos(glm::vec2 position, int mode)
{
    glm::vec2 returnValue;
    if (mode == 0)
    {
        returnValue.x = (int)(position.x / tileSize.x);
        returnValue.y = -1 * (int)(position.y / tileSize.y);
    }
    else if (mode < 0)
    {
        returnValue.x = std::floor(position.x / tileSize.x);
        returnValue.y = -1 * std::floor(position.y / tileSize.y);
    }
    else
    {
        returnValue.x = std::ceil(position.x / tileSize.x);
        returnValue.y = -1 * std::ceil(position.y / tileSize.y);
    }

    return returnValue;
}

void Map2D::FindTileID(int tileID, int level)
{
    if (level == -1) //if level is not set
    {
        level = currentLevel; //set level to the current level
    }

    for (int y = 0; y < tileMaps[level].size(); y++)
    {
        for (int x = 0; x < tileMaps[level][y].size(); x++)
        {
            if (tileMaps[level][y][x]->tileID == tileID)
            {
                tilesFound.push_back(glm::vec2(x, y)); //cache the tile column and row that we found
            }
        }
    }
}

glm::vec2 Map2D::TileToPos(glm::vec2 tilePosition)
{
    //since we are rendering the tiles downwards from 0, 0, we need to multiply the y by -1 to accurately get the world space coordinates
    return glm::vec2(tilePosition.x * tileSize.x, tilePosition.y * tileSize.y * -1);
}


void Map2D::SetTileMesh(Mesh* mesh)
{
	tileMesh = mesh;
}

Mesh* Map2D::GetTileMesh(void)
{
	return tileMesh;
}

void Map2D::SetTileSize(glm::vec2 newTileSize)
{
	if (newTileSize.x <= 0 || newTileSize.y <= 0) { return; }
	tileSize = newTileSize;
}

glm::vec2 Map2D::GetTileSize(void)
{
	return tileSize;
}

void Map2D::PerlinNoise1D(int nCount, float* fSeed, int nOctaves, float fBias, float* fOutput)
{
    for (int x = 0; x < nCount; x++) {
        float fNoise = 0.0f;
        float fScale = 1.0f;
        float fAccum = 0.0f;
        for (int o = 0; o < nOctaves; o++) {
            int nPitch = nCount >> o;
            if (nPitch == 0) break;

            int nSample1 = (x / nPitch) * nPitch;
            int nSample2 = (nSample1 + nPitch) % nCount;

            float fBlend = (float)(x - nSample1) / (float)nPitch;
            float fSample = (1.0f - fBlend) * fSeed[nSample1] + fBlend * fSeed[nSample2];
            fNoise += fSample * fScale;

            fAccum += fScale;
            fScale = fScale / fBias;
        }
        fOutput[x] = fNoise / fAccum;
    }
}

void Map2D::PerlinNoise2D(int nWidth, int nHeight, float* fSeed, int nOctaves, float fBias, float* fOutput)
{
    for (int x = 0; x < nWidth; x++) {
        for (int y = 0; y < nHeight; y++) {
            float fNoise = 0.0f;
            float fScale = 1.0f;
            float fAccum = 0.0f;

            for (int o = 0; o < nOctaves; o++) {
                int nPitch = nWidth >> o;
                if (nPitch == 0) break;

                int nSampleX1 = (x / nPitch) * nPitch;
                int nSampleY1 = (y / nPitch) * nPitch;

                int nSampleX2 = (nSampleX1 + nPitch) % nWidth;
                int nSampleY2 = (nSampleY1 + nPitch) % nHeight;

                float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
                float fBlendY = (float)(y - nSampleY1) / (float)nPitch;

                float fSampleT = (1.0f - fBlendX) * fSeed[nSampleY1 * nWidth + nSampleX1] + fBlendX * fSeed[nSampleY1 * nWidth + nSampleX2];
                float fSampleB = (1.0f - fBlendX) * fSeed[nSampleY2 * nWidth + nSampleX1] + fBlendX * fSeed[nSampleY2 * nWidth + nSampleX2];

                fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;

                fAccum += fScale;
                fScale = fScale / fBias;
            }
            fOutput[y * nWidth + x] = fNoise / fAccum;
        }
    }
}

//void Map2D::GenerateTileMap1D(int width, int height, int heightOffset, const float* fOutput, std::vector<std::vector<int>>& noiseTileMap)
//{
//    for (int x = 0; x < width; x++) {
//        int terrainHeight = -(fOutput[x] * height + heightOffset) + height + heightOffset; // Normalize to 0 to height range
//        terrainHeight = std::min(height - 1 - heightOffset, terrainHeight); // Ensure it's within bounds
//
//        for (int y = height - 1; y >= 0; y--) {
//            if (y >= terrainHeight + heightOffset) {
//                noiseTileMap[y][x] = STONE_ID; // Stone
//            }
//            else if (y == terrainHeight + heightOffset - 1) {
//                noiseTileMap[y][x] = DIRT_ID; // Dirt
//            }
//            else if (y > terrainHeight + heightOffset - 4) {
//                noiseTileMap[y][x] = GRASS_ID; // Grass
//            }
//            else {
//                noiseTileMap[y][x] = AIR_ID; // Air
//            }
//        }
//    }
//}

//void Map2D::GenerateTileMap2D(int width, int height, const float* fOutput, std::vector<std::vector<int>>& noiseTileMap, int minAirRange, int maxAirRange)
//{
//    for (int y = 0; y < height; y++) {
//        for (int x = 0; x < width; x++) {
//            int tileID = static_cast<int>(fOutput[y * width + x] * 12.0f);
//            if (tileID >= minAirRange && tileID <= maxAirRange)
//            {
//                noiseTileMap[y][x] = AIR_ID; //place air
//            }
//            else
//            {
//                noiseTileMap[y][x] = GRASS_ID; //place dirt
//            }
//        }
//    }
//}

void Map2D::GeneratePerlinMap(int levelID)
{
    int width = mapSizeX[levelID];
    int height = mapSizeY[levelID];
    int heightOffset = 100; // Adjust this value to shift the terrain up or down
    int nOctaves = 5; // Adjusted to a reasonable value
    float scalingBias = 1.2f;
    float scalingBias2D = 0.6f;

    std::vector<float> fSeed(width * height);
    std::vector<float> fOutput(width * height);

    // Seed the random number generator with the current time
    std::srand(static_cast<unsigned>(std::time(0)));

    // Generate seed values
    for (int i = 0; i < width * height; i++) {
        fSeed[i] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }

    // Generate 1D Perlin noise
    std::vector<float> fOutput1D(width);
    std::vector<float> fSeed1D(width);
    for (int i = 0; i < width; i++) {
        fSeed1D[i] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }
    PerlinNoise1D(width, fSeed1D.data(), nOctaves, scalingBias, fOutput1D.data());

    // Generate 2D Perlin noise
    PerlinNoise2D(width, height, fSeed.data(), nOctaves, scalingBias2D, fOutput.data());

    // Create a tile map
    std::vector<std::vector<int>> tileMap1D(height, std::vector<int>(width, 0));

    // Create a 2D tile map
    std::vector<std::vector<int>> tileMap2D(height, std::vector<int>(width, 0));

    // Generate the tile map based on Perlin noise
    GenerateTileMap1D(width, height, heightOffset, fOutput1D.data(), tileMap1D);
    GenerateTileMap2D(width, height, fOutput.data(), tileMap2D);

    WriteNoiseMap(tileMap1D, tileMap2D, levelID);
}

void Map2D::GenerateRandomSeed(std::vector<float> seed, unsigned int width)
{
    for (int i = 0; i < width; i++)
    {
        seed[i] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }
}

void Map2D::WriteNoiseMap(const std::vector<std::vector<int>>& tileMap1D, const std::vector<std::vector<int>>& tileMap2D, int levelID)
{
    tileMaps[levelID].resize(mapSizeY[levelID]);

    for (size_t y = 0; y < tileMap2D.size(); ++y)
    {
        for (size_t x = 0; x < tileMap2D[y].size(); ++x)
        {
            tileMaps[levelID][y].resize(mapSizeX[levelID]);

            OnWriteNoise(tileMap1D, tileMap2D, levelID, y, x);

            //if (tileMap1D[y][x] == AIR_ID) //if the tile we're writing to is an air block in the 1d tilemap
            //{
            //    if (y > 0 && tileMap2D[y - 1][x] != AIR_ID) //if we are not at the top of the map and the tile below is NOT air
            //    {
            //        tile* newTile = new tile(DIRT_ID);
            //        tileMaps[levelID][y][x] = newTile; //generate grass if dirt is exposed to air
            //    }
            //    else
            //    {
            //        tile* newTile = new tile(tileMap2D[y][x]);
            //        tileMaps[levelID][y][x] = newTile; //else generate the default 2D tilemap
            //    }
            //}
            //else
            //{
            //    if (tileMap1D[y][x] == GRASS_ID && (tileMap2D[y][x] != AIR_ID || (y > 0 && tileMap2D[y - 1][x] != AIR_ID))) //if the 2D tilemap is generating over 1D grass
            //    {
            //        tile* newTile = new tile(DIRT_ID);
            //        tileMaps[levelID][y][x] = newTile; //if so, we set the tilemap value to dirt (blends with the 2D perlin tilemap)
            //    }
            //    else
            //    {
            //        if (tileMap1D[y][x] == GRASS_ID && tileMap1D[y - 1][x] == GRASS_ID) //check if multiple layers of grass are being rendered
            //        {
            //            tile* newTile = new tile(DIRT_ID);
            //            tileMaps[levelID][y][x] = newTile; //set the tile to dirt if the tile above is grass and the tile itself is grass
            //        }
            //        else
            //        {
            //            tile* newTile = new tile(tileMap1D[y][x]);
            //            tileMaps[levelID][y][x] = newTile;
            //        }
            //    }
            //}
        }
    }
}

void Map2D::WriteMapFromCSV(const std::string& filename, int levelID)
{
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open())
    {
        std::cout << "Could not open file: " << filename << std::endl;
        return;
    }

    // Clear the existing tile map
    for (int y = 0; y < tileMaps[levelID].size(); y++)
    {
        for (int x = 0; x < tileMaps[levelID][y].size(); x++)
        {
            if (tileMaps[levelID][y][x] != nullptr)
            {
                delete tileMaps[levelID][y][x]; // Clean up dynamically allocated tiles
            }
        }
    }
    tileMaps[levelID].clear();

    // Initialize the outer vector to the correct size
    tileMaps[levelID].resize(mapSizeY[levelID]);

    for (int y = 0; y < mapSizeY[levelID]; y++)
    {
        std::getline(file, line);
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<int> row;

        for (int x = 0; x < mapSizeX[levelID]; x++)
        {
            std::getline(lineStream, cell, ',');

            if (cell == "")
            {
                row.push_back(AIR_ID);
            }
            else
            {
                row.push_back(std::stoi(cell));
            }
        }

        // Initialize the inner vector to the correct size
        tileMaps[levelID][y].resize(mapSizeX[levelID]);

        for (int x = 0; x < mapSizeX[levelID]; x++)
        {
            Tile* newTile = new Tile(row[x]);
            tileMaps[levelID][y][x] = newTile;
        }
    }
}


void Map2D::SaveMapToCSV(const std::string filename, int levelID)
{
    std::ofstream file(filename); //open our CSV file for writing

    for (size_t y = 0; y < tileMaps[levelID].size(); ++y) //inverting y and x ensures we write it in rows instead of in columns
    {
        for (size_t x = 0; x < tileMaps[levelID][y].size(); ++x)
        {
            if (tileMaps[levelID][y][x]->tileID == AIR_ID)
            {
                file << ""; //if tilemap data is air, write a blank value to the csv file
            }
            else
            {
                file << tileMaps[levelID][y][x]->tileID; //write the tilemap data to the file
            }

            if (x < tileMaps[levelID][y].size() - 1) { //so long as we are not at the last index, go to the adjacent right column
                file << ",";
            }
        }
        file << "\n"; //when 1 row is complete, go to a new line
    }

    file.close(); //at the end, close the file
}

void Map2D::ChangeLevel(int newLevel)
{
    if (currentLevel != maxLevels && currentLevel == newLevel) { return; } //ensure we do not re-render the map if we try to change to the same map

    currentLevel = newLevel;
}

int Map2D::GetCurrentLevel(void)
{
    return currentLevel;
}



void Map2D::PrintMapData(int level)
{
    std::cout << "printing map..." << std::endl;


    for (int y = 0; y < mapSizeY[level]; y++)
    {
        for (int x = 0; x < mapSizeX[level]; x++)
        {
            std::cout << tileMaps[level][y][x]->tileID << ", ";
        }
        
        std::cout << "\n";
    }
}

void Map2D::SetTextureToID(unsigned int ID, const char* textureName)
{
    unsigned int textureID;
    textureID = ImageLoader::GetInstance()->LoadTextureGetID(textureName, true);

    if (textureID == 0)
    {
        std::cout << "unable to load texture: " << textureName << std::endl;
        return;
    }

    textureMaps.insert(std::pair<int, int>(ID, textureID));
}

void Map2D::SetPassableTile(unsigned int ID, bool isPassable)
{
    passableTiles.insert(std::pair<int, bool>(ID, isPassable));
}

void Map2D::SetDangerousTile(unsigned int ID, bool isDangerous)
{
    dangerousTiles.insert(std::pair<int, bool>(ID, isDangerous));
}

void Map2D::GenerateNodes()
{
    for (int i = 0; i < maxLevels; i++) //iterate through each level
    {
        AStar* newAStar = new AStar(tileSize);
        newAStar->Init(mapSizeX[i], mapSizeY[i]); // call the AStar init function for each level
        AStarNodes.push_back(newAStar); // add the new AStar ptr to the list
    }
}


Map2D::Map2D(const int MaxLevels) : maxLevels(maxLevels)
{
    currentLevel = MaxLevels;
}

Map2D::~Map2D(void)
{
    for (int i = 0; i < maxLevels; i++)
    {
        for (int y = 0; y < tileMaps[i].size(); y++)
        {
            for (int x = 0; x < tileMaps[i][y].size(); x++)
            {
                delete tileMaps[i][y][x]; //delete each tile in a row on destroy to prevent memory leak
            }

            tileMaps[i][y].clear(); //clear data from each row when all tiles have been deleted
        }
        tileMaps[i].clear(); //clear data from each map when all tiles in a map have been cleared
    }
    //delete tileMaps;
}

void Map2D::Init(int startLevel) {
    currentLevel = startLevel;

    //setTextureToID(0, "Image/boxTransparent.tga");
    SetPassableTile(0, true); //air is passable

    BindTextures();

    for (int i = 0; i < maxLevels; i++)
    {
        for (int y = 0; y < (int)mapSizeY[i]; y++)
        {
            for (int x = 0; x < (int)mapSizeX[i]; x++)
            {
                if (passableTiles.count(tileMaps[i][y][x]->tileID) > 0) {
                    tileMaps[i][y][x]->isPassable = passableTiles.at(tileMaps[i][y][x]->tileID);
                    std::cout << "set tile of ID: " << tileMaps[i][y][x]->tileID << " to passable" << std::endl;
                }

                if (dangerousTiles.count(tileMaps[i][y][x]->tileID) > 0) {
                    tileMaps[i][y][x]->isDangerous = dangerousTiles.at(tileMaps[i][y][x]->tileID);
                }

                if (tileMaps[i][y][x]->tileID == AIR_ID) { 
                    tileMaps[i][y][x]->isPassable = true;
                    continue; 
                
                }

                tileMaps[i][y][x]->setPosition(glm::vec2(x * tileSize.x, -y * tileSize.y));
                tileMaps[i][y][x]->setScale(tileSize);



                tileMaps[i][y][x]->currentLevel = i;

                tileMaps[i][y][x]->setMesh(tileMesh);
                tileMaps[i][y][x]->textureID = textureMaps.at(tileMaps[currentLevel][y][x]->tileID);
            }
        }
    }

}

void Map2D::Update()
{
    //reset all our map update alerts each frame
    updatedTileXindex.clear();
    updatedTileYindex.clear();
    tilesFound.clear();
    mapUpdate = false;
}

void Map2D::Render(glm::vec2 cameraPos, glm::vec2 renderDistance)
{
    glm::vec2 cameraTile = PosToTilePos(cameraPos);

    if (renderDistance.x < 0)
    {
        renderDistance.x = 0;
    }
    if (renderDistance.y < 0)
    {
        renderDistance.y = 0;
    }

    int camX = cameraTile.x;
    int camY = cameraTile.y;

    int renderX = std::ceil(renderDistance.x);
    int renderY = std::ceil(renderDistance.y);

    for (int y = std::max(camY - renderY, 0); y < std::min(camY + renderY, (int)mapSizeY[currentLevel]); y++)
    {
        for (int x = std::max(camX - renderX, 0); x < std::min(camX + renderX, (int)mapSizeX[currentLevel]); x++)
        {
            if (tileMaps[currentLevel][y][x]->tileID == AIR_ID) { continue; }
            tileMaps[currentLevel][y][x]->Render();
        }
    }
}