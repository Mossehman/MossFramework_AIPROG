#include "SceneAI.h"

#define WIDTH 1200
#define HEIGHT 800

bool SceneAI::Init()
{
    RenderParameters::GetInstance()->Init(glm::vec4(0, 0, 1, 0), "Shader//comg.vertexshader", "Shader//comg.fragmentshader");
    camera.Init(Vector3(300, -300, 1), Vector3(300, -300, 0), Vector3(0, 1, 0));


    return true;
}

bool SceneAI::Update(double dt)
{
    return true;
}

void SceneAI::Render()
{
    RenderParameters::GetInstance()->Render(camera, WIDTH, HEIGHT);
}

void SceneAI::Exit()
{
}
