#include "GameStateAI.h"
#include <RenderParameters.h>
#include <Map2D.h>
#include <KeyboardController.h>
#include "TestLevel.h"
#include "TestMessage.h"


bool GameStateAI::Init()
{
    RenderParameters::GetInstance()->Init(glm::vec4(0.f, 0.f, 1.f, 0.8f), "Shader//comg.vertexshader", "Shader//comg.fragmentshader");
    camera.Init(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
     
    Map2D::GetInstance()->AddLevel(LEVEL1, new TestLevel(60, 55, glm::vec2(20, 20), "Maps/LeagueMap.csv"));
    Map2D::GetInstance()->GetInstance()->Init(LEVEL1);

    go1 = new GameObject();
    go1->SetMesh(MeshBuilder::GenerateQuad("quad", Color(1, 0, 1)));
    //go1->SetTexture("Image/grass.tga");

    go1->setPosition(glm::vec2(-30, 0));
    go1->setScale(glm::vec2(80, 80));
    go1->setOpacity(0.7f);

    go2 = new GameObject();

    go2->SetMesh(MeshBuilder::GenerateQuad("quad", Color(0, 1, 1)));
    go2->SetTexture("Image/dirt.tga", 30, 1);

    go2->setPosition(glm::vec2(30, 30));
    go2->setScale(glm::vec2(80, 80));
    //go2->setOpacity(0.4f);

    go2->AddAnimation("Test", new AnimatedSpriteParam(0, 29));
    go2->SetCurrentAnimation("Test");

    newEntity = new TestEntity();
    newEntity->SetMesh(MeshBuilder::GenerateQuad("quad", Color(1, 0, 1)));
    newEntity->SetTexture("Image/grass.tga");
    newEntity->setPosition(glm::vec2(0, 0));
    newEntity->setScale(glm::vec2(80, 80));
    newEntity->setOpacity(0.7f);

    newEntity2 = new EntityAI2D();
    newEntity2->SetMesh(MeshBuilder::GenerateQuad("quad", Color(1, 0, 1)));
    newEntity2->SetTexture("Image/stone.tga");
    newEntity2->setPosition(glm::vec2(0, 0));
    newEntity2->setScale(glm::vec2(80, 80));
    newEntity2->setOpacity(0.7f);

    go1->Init(GameObjectList, LEVEL1);
    go2->Init(GameObjectList, LEVEL1);
    newEntity->Init(GameObjectList, LEVEL1, false, false, false, false);
    newEntity2->Init(GameObjectList, LEVEL1, false, false, false, false);


    newEntity->targetPos = glm::vec2(300, -80);
    newEntity->InitAStar();
    newEntity->SolveAStar();

    newEntity2->targetPos = glm::vec2(600, -150);
    newEntity2->InitAStar();
    newEntity2->SolveAStar();


    MessageHub::GetInstance()->AddReciever("EntityTest", newEntity);
    MessageHub::GetInstance()->AddReciever("EntityTest2", newEntity2);
    MessageHub::GetInstance()->SendMsg(new BaseMessage("EntityTest", 1)); 

    return true;
}

bool GameStateAI::Update(double dt)
{
    if (CKeyboardController::GetInstance()->IsKeyDown('X'))
    {
        Map2D::GetInstance()->ChangeLevel(LEVEL2);
    }

    if (CKeyboardController::GetInstance()->IsKeyDown('V'))
    {
        Map2D::GetInstance()->ChangeLevel(LEVEL1);
    }

    for (int i = 0; i < GameObjectList.size(); i++)
    {
        GameObjectList[i]->Update(dt);
        GameObjectList[i]->UpdateAnimations(dt);
    }

    MessageHub::GetInstance()->Update();

    return true;
}

void GameStateAI::Render()
{
    RenderParameters::GetInstance()->Render(camera, 1200, 800);

    glDepthMask(GL_FALSE);
    for (int i = 0; i < GameObjectList.size(); i++)
    {
        GameObjectList[i]->Render();
    }

    Map2D::GetInstance()->Render(camera.position, glm::vec2(100, 100));
    //Map2D::GetInstance()->RenderNodes();

    newEntity->RenderNodePath(Color(1, 0, 0));
    newEntity2->RenderNodePath(Color(0, 1, 1));
    glDepthMask(GL_TRUE);

}

void GameStateAI::Destroy()
{
}
