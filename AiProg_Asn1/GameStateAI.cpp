#include "GameStateAI.h"
#include <RenderParameters.h>
#include <Map2D.h>

bool GameStateAI::Init()
{
    RenderParameters::GetInstance()->Init(glm::vec4(0.f, 0.f, 1.f, 0.8f), "Shader//comg.vertexshader", "Shader//comg.fragmentshader");
    camera.Init(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
     
    map->GetInstance()->SetTileMesh(MeshBuilder::GenerateQuad("Quad", Color(1.f, 1.f, 1.f), 1.0f));
    map->GetInstance()->SetTileSize(glm::vec2(30, 30));
    map->GetInstance()->mapSizeX[LEVEL1] = 30;
    map->GetInstance()->mapSizeY[LEVEL1] = 30;
    //map->::GetInstance()->generatePerlinMap(LevelIDs::SUPERDESTROYER);
    map->GetInstance()->WriteMapFromCSV("Maps/tilemap1.csv", LEVEL1);
    map->GetInstance()->Init(LEVEL1);
    map->GetInstance()->GenerateNodes();

    go1 = new GameObject();
    go1->SetMesh(MeshBuilder::GenerateQuad("quad", Color(1, 0, 1)));
    go1->SetTexture("Image/grass.tga");

    go1->setPosition(glm::vec2(-30, 0));
    go1->setScale(glm::vec2(80, 80));
    go1->setOpacity(0.7f);

    go2 = new GameObject();
    go2->SetMesh(MeshBuilder::GenerateQuad("quad", Color(0, 1, 1)));
    go2->SetTexture("Image/dirt.tga");

    go2->setPosition(glm::vec2(30, 30));
    go2->setScale(glm::vec2(80, 80));
    go2->setOpacity(0.4f);

    return true;
}

bool GameStateAI::Update(double dt)
{
    return true;
}

void GameStateAI::Render()
{
    RenderParameters::GetInstance()->Render(camera, 1200, 800);

    glDepthMask(GL_FALSE);
    go1->Render();
    go2->Render();

    map->Render(camera.position, glm::vec2(30, 30));
    map->RenderNodes();
    glDepthMask(GL_TRUE);

}

void GameStateAI::Destroy()
{
}
