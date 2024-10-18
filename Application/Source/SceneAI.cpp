#include "SceneAI.h"

#define WIDTH 1200
#define HEIGHT 800

bool SceneAI::Init()
{
    RenderParameters::GetInstance()->Init(glm::vec4(0, 0, 1, 0), "Shader//comg.vertexshader", "Shader//comg.fragmentshader");
    camera.Init(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    go = new GameObject();
    go->setMesh(MeshBuilder::GenerateQuad("GO", Color(1, 1, 1)));

    return true;
}

bool SceneAI::Update(double dt)
{
    std::cout << go->getPosition().x << ", " << go->getPosition().y << std::endl;
    return true;
}

void SceneAI::Render()
{
    RenderParameters::GetInstance()->Render(camera, WIDTH, HEIGHT);
    go->Render();
}

void SceneAI::Exit()
{
}
