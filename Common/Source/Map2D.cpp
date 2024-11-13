#include "Map2D.h"
#include <fstream>
#include <sstream>
#include "ImageLoader.h"


int Map2D::GetMapInfo(unsigned int row, unsigned int column, int levelToCheck)
{
	if (levelToCheck == -1) //if we are not checking a specific level
	{
		levelToCheck = currentLevel; //check the currently active level
	}

    if (column >= GetLevel()->GetMapX() || row >= GetLevel()->GetMapY()) //check if tile or row is out of range
    {
        return 0;
    }

    return GetLevel()->GetTilemap()[row][column]->tileID;
}

bool Map2D::GetMapPassable(unsigned int row, unsigned int column, int levelToCheck)
{
    if (levelToCheck == -1) //if we are not checking a specific level
    {
        levelToCheck = currentLevel; //check the currently active level
    }

    if (column >= GetLevel()->GetMapX() || row >= GetLevel()->GetMapY()) //check if tile or row is out of range
    {
        return false;
    }

    return GetLevel()->GetTilemap()[row][column]->isPassable;
}


void Map2D::SetMapInfo(unsigned int row, unsigned int column, unsigned int newTileID, int levelToCheck)
{
    if (levelToCheck == -1) //if we are not checking a specific level
    {
        levelToCheck = currentLevel; //check the currently active level
    }

    if (column > GetLevel(levelToCheck)->GetMapX() || row > GetLevel(levelToCheck)->GetMapY()) //check if tile or row is out of range
    {
        return;
    }
    //if (GetLevel(levelToCheck)->GetTilemap()[row][column]->tileID == newTileID) { return; } //tile to replace is the same as the one on the current map, do not replace


    GetLevel(levelToCheck)->GetTilemap()[row][column]->tileID = newTileID;

    //cache the modified row and column values to better optimise other systems (clear this at the end of every frame)
    updatedTileXindex.push_back(column);
    updatedTileYindex.push_back(row);

    mapUpdate = true;
}

glm::ivec2 Map2D::PosToTilePos(glm::vec2 position, int mode)
{
    glm::ivec2 returnValue;
    if (mode == 0)
    {
        returnValue.x = (int)(position.x / GetLevel()->GetTileSize().x);
        returnValue.y = -1 * (int)(position.y / GetLevel()->GetTileSize().y);
    }
    else if (mode < 0)
    {
        returnValue.x = std::floor(position.x / GetLevel()->GetTileSize().x);
        returnValue.y = -1 * std::floor(position.y / GetLevel()->GetTileSize().y);
    }
    else
    {
        returnValue.x = std::ceil(position.x / GetLevel()->GetTileSize().x);
        returnValue.y = -1 * std::ceil(position.y / GetLevel()->GetTileSize().y);
    }

    return returnValue;
}

void Map2D::FindTileID(int tileID, int level)
{
    if (level == -1) //if level is not set
    {
        level = currentLevel; //set level to the current level
    }

    for (int y = 0; y < GetLevel(level)->GetMapX(); y++)
    {
        for (int x = 0; x < GetLevel(level)->GetMapY(); x++)
        {
            if (GetLevel(level)->GetTilemap()[y][x]->tileID == tileID)
            {
                tilesFound.push_back(glm::vec2(x, y)); //cache the tile column and row that we found
            }
        }
    }
}

glm::vec2 Map2D::TileToPos(glm::vec2 tilePosition)
{
    //since we are rendering the tiles downwards from 0, 0, we need to multiply the y by -1 to accurately get the world space coordinates
    return glm::vec2(tilePosition.x * GetLevel()->GetTileSize().x, tilePosition.y * GetLevel()->GetTileSize().y * -1);
}




void Map2D::ChangeLevel(int newLevel)
{
    if (currentLevel != maxLevels && currentLevel == newLevel) { return; } //ensure we do not re-render the map if we try to change to the same map
    currentLevel = newLevel;
    this->GenerateNodes();
}

int Map2D::GetCurrentLevel(void)
{
    return currentLevel;
}



void Map2D::PrintMapData(int level)
{
    std::cout << "printing map..." << std::endl;


    for (int y = 0; y < GetLevel(level)->GetMapY(); y++)
    {
        for (int x = 0; x < GetLevel(level)->GetMapX(); x++)
        {
            std::cout << GetLevel(level)->GetTilemap()[y][x]->tileID << ", ";
        }
        
        std::cout << "\n";
    }
}

void Map2D::GenerateNodes()
{
    if (AStarNodes) { delete AStarNodes; }
    AStarNodes = new AStar(GetLevel()->GetTileSize());
    AStarNodes->Init(GetLevel()->GetMapX(), GetLevel()->GetMapY()); // call the AStar init function for each level
}

void Map2D::RenderNodes()
{
    std::vector<GameObject*> nodeGOList;
    for (int i = 0; i < AStarNodes->GetNodes().size(); i++)
    {
        GameObject* nodeGO = new GameObject(AStarNodes->GetNodes()[i]->Position);
        nodeGO->setScale(glm::vec2(20, 20));
        nodeGO->SetMesh(MeshBuilder::GenerateQuad("Node Mesh", Color(1, 0, 0)));
        nodeGO->setOpacity(0.4f);
        nodeGO->Render();
        nodeGOList.push_back(nodeGO);
    }

    for (int i = 0; i < nodeGOList.size(); i++)
    {
        delete nodeGOList[i];
    }
    nodeGOList.clear();
}

AStar* Map2D::GetAStar()
{
    return AStarNodes;
}

void Map2D::AddLevel(int levelID, BaseLevel* newLevel, Mesh* tileMesh)
{
    if (Levels.count(levelID > 0)) { return; }
    newLevel->SetTileMesh(tileMesh);
    Levels.insert(std::pair<int, BaseLevel*>(levelID, newLevel));
}

BaseLevel* Map2D::GetLevel(int levelID)
{
    if (levelID == -1) { levelID = currentLevel; }
    if (Levels.count(levelID) <= 0) { return nullptr; }

    return Levels.at(levelID);
}

Map2D::~Map2D()
{
    for (auto it = Levels.begin(); it != Levels.end(); ++it)
    {
        BaseLevel* thisLevel = it->second;
        thisLevel->ClearMap(); //iterate through each map and clear the tiles

        delete thisLevel;
    }

    Levels.clear();

}

void Map2D::Init(int startLevel) {
    currentLevel = startLevel;

    for (auto it = Levels.begin(); it != Levels.end(); ++it)
    {
        BaseLevel* thisLevel = it->second;
        thisLevel->Init();
    }

    GenerateNodes();

}

void Map2D::Update(float dt)
{
    GetLevel()->Update(dt);

    //reset all our map update alerts each frame
    updatedTileXindex.clear(); 
    updatedTileYindex.clear(); 
    tilesFound.clear();  
    mapUpdate = false; 
}

void Map2D::Render(glm::vec2 cameraPos, glm::ivec2 renderDistance)
{
    glm::ivec2 CameraTile = PosToTilePos(cameraPos);
    GetLevel()->Render(CameraTile, renderDistance);

}