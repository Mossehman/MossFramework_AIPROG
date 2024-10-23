#include "GameStateAI.h"
#include <RenderParameters.h>
#include <Map2D.h>

bool GameStateAI::Init()
{
    RenderParameters::GetInstance()->Init(glm::vec4(0.f, 0.f, 1.f, 0.8f), "Shader//comg.vertexshader", "Shader//comg.fragmentshader");
    camera.Init(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));


    go1 = new GameObject();
    go1->setMesh(MeshBuilder::GenerateQuad("quad", Color(1, 0, 1)));
    //go1->setTexture("Image/grass.tga");

    go1->setPosition(glm::vec2(-30, 0));
    go1->setScale(glm::vec2(80, 80));
    go1->setOpacity(0.7f);

    go2 = new GameObject();
    go2->setMesh(MeshBuilder::GenerateQuad("quad", Color(0, 1, 1)));
    //go1->setTexture("Image/grass.tga");

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
    glDepthMask(GL_TRUE);

}

void GameStateAI::Destroy()
{
}
