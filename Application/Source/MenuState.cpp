#include "MenuState.h"
#include "RenderParameters.h"
#include "KeyboardController.h"
#include "GameStateManager.h"
#include "Camera.h"
#include "GL\glew.h"

#define WIDTH 1200
#define HEIGHT 800

bool MenuState::Init()
{
    RenderParameters::GetInstance()->Init(glm::vec4(0.f, 1.f, 0.f, 1.f), "Shader//comg.vertexshader", "Shader//comg.fragmentshader");
    camera.Init(glm::vec3(300, -300, 1), glm::vec3(300, -300, 0), glm::vec3(0, 1, 0));
    return true;
}

bool MenuState::Update(double dt)
{
    timer += dt;
    if (timer < 5) { return true; }
    glClearColor(1.f, 1.f, 0.f, 1.f);

    if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_SPACE))
    {
        GameStateManager::GetInstance()->setCurrentState("PlayState");
    }

    return true;
}

void MenuState::Render()
{
    RenderParameters::GetInstance()->Render(camera, WIDTH, HEIGHT);
}

void MenuState::Destroy()
{
    return;
}
