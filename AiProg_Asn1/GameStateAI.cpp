#include <sstream>

#include "GameStateAI.h"
#include <RenderParameters.h>
#include <Map2D.h>
#include <KeyboardController.h>
#include "TestLevel.h"
#include "TestMessage.h"

#include "MouseController.h"
#include "Application.h"

#include "TestState.h"
#include "TestState2.h"

#include "MsgLane.h"
#include "MsgSpawn.h"
#include "MsgGameOver.h"
#include "MsgDied.h"
#include "MsgGetRespawnTime.h"
#include "MsgDestroy.h"
#include "MsgGetEntityCount.h"

#include "ChaseState.h"
#include "RetreatState.h"
#include "MoveState.h"
#include "AttackTurretState.h"
#include "AttackState.h"
#include "ChaseState.h"
#include "InitLaneState.h"
#include "TurretIdleState.h"
#include "SpawnState.h"
#include "TurretDamagedState.h"
#include "InhibitorTurretDeadState.h"
#include "StunnedState.h"
#include "NPCDeadState.h"
#include "DeadState.h"

#include "ProjectileSkill.h"
#include "DashSkill.h"

#include "GameManager.h"

#include "EntityTypes.h"
#include <random>
#include "GameStateManager.h"
#include "RespawnState.h"

#define SUDDEN_DEATH_TIME 200


bool GameStateAI::Init()
{
    RenderParameters::GetInstance()->Init(glm::vec4(0.f, 0.f, 0.3f, 0.8f), "Shader//comg.vertexshader", "Shader//comg.fragmentshader");
    camera.Init(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    camera.ToClamp = false;
     
    Map2D::GetInstance()->AddLevel(LEVEL1, new TestLevel(60, 55, glm::vec2(45, 30), "Maps/LeagueMap.csv"));
    Map2D::GetInstance()->GetInstance()->Init(LEVEL1);

    this->TurretsInit();
    this->EntitiesInit();

    MessageHub::GetInstance()->AddReciever("GameScene", this);
    textMesh->textureID = LoadTGA("Image//calibri.tga"); 

    return true;
}

bool GameStateAI::Update(double dt)
{

    if (CKeyboardController::GetInstance()->IsKeyDown('Q'))
    {
        pauseSim = false;
    }

    CameraControls(dt, 400.0f);
    camera.Update(dt);

    if (pauseSim) { return true; }
    FPS = 1 / dt;
    timeElapsed += dt;
    //MsgGetEntityCount* newMsg = new MsgGetEntityCount({ "Unit" }, UNIT, 1);

    //for (GameObject* obj : GameObjectList)
    //{
    //    NumRed += obj->HandleMessage(newMsg);
    //}

    respawnTimer += dt;
    if (respawnTimer >= TIME_BEFORE_RESPAWN_INCREASE)
    {
        respawnTimer = 0.0f;
        respawnTime += 15.0f;
    }

    if (timeElapsed > SUDDEN_DEATH_TIME)
    {
        MessageHub::GetInstance()->SendMsg(new MsgDestroy({ "TowersTop", "TowersMid", "TowersBot" }));
    }

    for (int i = 0; i < GameObjectList.size(); i++)
    {
        GameObjectList[i]->Update(dt);
        GameObjectList[i]->UpdateAnimations(dt);
        
    }

    MessageHub::GetInstance()->Update();

    if (CKeyboardController::GetInstance()->IsKeyDown('C'))
    {
        GameStateManager::GetInstance()->speedMultiplier += 0.1f;
    }
    else if (CKeyboardController::GetInstance()->IsKeyDown('V'))
    {
        GameStateManager::GetInstance()->speedMultiplier -= 0.1f;
    }

    if (GameStateManager::GetInstance()->speedMultiplier > 2.5) { GameStateManager::GetInstance()->speedMultiplier = 2.5; }
    else if (GameStateManager::GetInstance()->speedMultiplier < 1) { GameStateManager::GetInstance()->speedMultiplier = 1; }

    return true;
}

void GameStateAI::Render()
{
    std::ostringstream ss;

    RenderParameters::GetInstance()->Render(camera, 1200, 800);

    glDepthMask(GL_FALSE);
    Map2D::GetInstance()->Render(camera.position, glm::vec2(100, 100));
    for (int i = 0; i < GameObjectList.size(); i++)
    {
        GameObjectList[i]->Render();
    }
    glDepthMask(GL_TRUE);

    ss.str("");
    ss.precision(5);
    ss << "FPS:" << FPS;
    RenderTextOnScreen(textMesh, ss.str(), Color(0, 1, 0), 3, 50, 6);

    ss.str("");
    ss << "Winner: " << winner;
    RenderTextOnScreen(textMesh, ss.str(), Color(0, 1, 0), 3, 50, 10);

    ss.str("");
    ss << "SpeedMul: " << GameStateManager::GetInstance()->speedMultiplier;
    RenderTextOnScreen(textMesh, ss.str(), Color(0, 1, 0), 3, 50, 14);

    ss.str("");
    ss << "Elapsed time: " << timeElapsed;
    RenderTextOnScreen(textMesh, ss.str(), Color(0, 1, 0), 3, 50, 18);

    if (timeElapsed > SUDDEN_DEATH_TIME)
    {
        RenderTextOnScreen(textMesh, "Sudden death!", Color(1, 0, 0), 3, 50, 22);
    }
    else
    {
        float timeToSuddenDeath = SUDDEN_DEATH_TIME - timeElapsed;
        ss.str("");
        ss << "Time to Sudden Death: " << timeToSuddenDeath;
        RenderTextOnScreen(textMesh, ss.str(), Color(0, 1, 0), 3, 50, 22);
    }

}

void GameStateAI::Destroy()
{
    for (int i = 0; i < GameObjectList.size(); i++)
    {
        delete GameObjectList[i];
    }
    GameObjectList.clear();
}

void GameStateAI::GenerateTurrets(int tileID, std::string laneID, int team)
{
    std::vector<glm::vec2> TurretTeamPos;

    TurretTeamPos = Map2D::GetInstance()->GetLevel()->GetTilesWithID(tileID);
    if (TurretTeamPos.empty()) { return; }

    for (int i = 0; i < TurretTeamPos.size(); i++)
    {
        LOLEntity* Turret = new LOLEntity(TURRET);
        Turret->setPosition(TurretTeamPos[i]);
        Turret->Team = team;
        Turret->setScale(glm::vec2(Map2D::GetInstance()->GetLevel()->GetTileSize().x * 1.5f, Map2D::GetInstance()->GetLevel()->GetTileSize().y * 2.2f));
        Turret->SetMesh(MeshBuilder::GenerateQuad("quad", Color(1, 1, 1)));
        if (team == 1)
        {
            Turret->SetTexture("Image/Turret_Red.png");
        }
        else
        {
            Turret->SetTexture("Image/Turret_Blue.png");
        }
        MessageHub::GetInstance()->AddReciever(laneID, Turret);
        MessageHub::GetInstance()->AddReciever("Towers", Turret);

        Turret->MaxHealth = 200;
        Turret->health = 200;

        Turret->GetFSM()->AddState("Move", new TurretIdleState());
        Turret->GetFSM()->AddState("Attack", new AttackState());
        Turret->GetFSM()->AddState("Damaged", new TurretDamagedState());
        Turret->GetFSM()->AddState("Dead", new NPCDeadState());
        Turret->CanRegenHP = false;
        Turret->AddSkill(new ProjectileSkill({ ATTACK }, OPPOSING_TEAMS));

        Turret->Init(GameObjectList, Map2D::GetInstance()->GetCurrentLevel(), false, false, false, false);
    }
}

void GameStateAI::TurretsInit()
{
    std::vector<glm::vec2> InhibitorTurretsPos;
    InhibitorTurretsPos = Map2D::GetInstance()->GetLevel()->GetTilesWithID(INHIBITOR_INDEX);
    if (InhibitorTurretsPos.size() < 2) { return; }
    for (int i = 0; i < InhibitorTurretsPos.size(); i++)
    {
        LOLEntity* InhibitorTurret = new LOLEntity(MAIN_TURRET);
        InhibitorTurret->setPosition(InhibitorTurretsPos[i]);
        InhibitorTurret->Team = i + 1;
        InhibitorTurret->setScale(glm::vec2(Map2D::GetInstance()->GetLevel()->GetTileSize().x * 3.0f, Map2D::GetInstance()->GetLevel()->GetTileSize().y * 4.5f));
        InhibitorTurret->SetMesh(MeshBuilder::GenerateQuad("quad", Color(1, 1, 1)));
        if (InhibitorTurret->Team == 1)
        {
            InhibitorTurret->SetTexture("Image/Turret_Red.png");
        }
        else
        {
            InhibitorTurret->SetTexture("Image/Turret_Blue.png");
        }

        InhibitorTurret->GetFSM()->AddState("Move", new TurretIdleState());
        InhibitorTurret->GetFSM()->AddState("Attack", new AttackState());
        InhibitorTurret->GetFSM()->AddState("Spawn", new SpawnState());
        InhibitorTurret->GetFSM()->AddState("Damaged", new TurretDamagedState());
        InhibitorTurret->GetFSM()->AddState("Dead", new InhibitorTurretDeadState());
        InhibitorTurret->AddSkill(new ProjectileSkill({ ATTACK }, OPPOSING_TEAMS));
        InhibitorTurret->CanRegenHP = false;

        InhibitorTurret->TeamStrengthModifier = 1.5f;
        InhibitorTurret->MaxHealth = 500;
        InhibitorTurret->health = 500;

        InhibitorTurret->Init(GameObjectList, Map2D::GetInstance()->GetCurrentLevel(), false, false, false, false);
        MessageHub::GetInstance()->AddReciever("Towers", InhibitorTurret);
        MessageHub::GetInstance()->AddReciever("InhibitorTower", InhibitorTurret);
    }
    this->Team1SpawnPos = InhibitorTurretsPos[0];
    this->Team2SpawnPos = InhibitorTurretsPos[1];


    this->GenerateTurrets(TEAM_ONE_TURRET_INDEX_T, "TowersTop", 1);
    this->GenerateTurrets(TEAM_ONE_TURRET_INDEX_M, "TowersMid", 1);
    this->GenerateTurrets(TEAM_ONE_TURRET_INDEX_B, "TowersBot", 1);

    this->GenerateTurrets(TEAM_TWO_TURRET_INDEX_T, "TowersTop", 2);
    this->GenerateTurrets(TEAM_TWO_TURRET_INDEX_M, "TowersMid", 2);
    this->GenerateTurrets(TEAM_TWO_TURRET_INDEX_B, "TowersBot", 2);

}

void GameStateAI::EntitiesInit()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-YSpawnRandomRange, YSpawnRandomRange);

    std::vector<int> cachedOffsets;
    for (int i = 0; i < MAX_TEAM_COUNT; i++)
    {
        int yOffset = dis(gen);
        bool closeCheck = false;

        while (!closeCheck && !cachedOffsets.empty())
        {
            closeCheck = true;
            for (int offsets : cachedOffsets)
            {
                if (abs(yOffset - offsets) <= 2) {
                    closeCheck = false;
                    yOffset = dis(gen);
                    break;
                }
            }
        }

        cachedOffsets.push_back(yOffset);


        float randomYOffset = yOffset * Map2D::GetInstance()->GetLevel()->GetTileSize().y;
        LOLEntity* Champion = new LOLEntity(UNIT);

        Champion->Team = 1;
        Champion->spawnPos = glm::vec2(Team1SpawnPos.x, Team1SpawnPos.y + randomYOffset);
        Champion->setPosition(glm::vec2(Team1SpawnPos.x, Team1SpawnPos.y + randomYOffset));
        Champion->setScale(glm::vec2(Map2D::GetInstance()->GetLevel()->GetTileSize().x, Map2D::GetInstance()->GetLevel()->GetTileSize().y));
        Champion->SetMesh(MeshBuilder::GenerateQuad("quad", Color(1, 1, 1)));
        Champion->SetTexture("Image/Minion_Red.png");
        MessageHub::GetInstance()->AddReciever("Units", Champion);

        Champion->GetFSM()->AddState("Move", new MoveState());
        Champion->GetFSM()->AddState("InitLane", new InitLaneState());
        Champion->GetFSM()->AddState("AttackTurret", new AttackTurretState());
        Champion->GetFSM()->AddState("Attack", new AttackState());
        Champion->GetFSM()->AddState("Chase", new ChaseState());
        Champion->GetFSM()->AddState("Retreat", new RetreatState());
        Champion->GetFSM()->AddState("Damaged", new StunnedState());
        Champion->GetFSM()->AddState("Dead", new DeadState());
        Champion->GetFSM()->AddState("Respawn", new RespawnState());
        Champion->AddSkill(new ProjectileSkill({ ATTACK }, OPPOSING_TEAMS));
        Champion->AddSkill(new DashSkill({ MOVEMENT }));
        //Champion->AddSkill(new DashSkill({ MOVEMENT }));

        Champion->Init(GameObjectList, Map2D::GetInstance()->GetCurrentLevel(), false, false, false, false);
        Champion->InitAStar();

        int lane = 0;
        if (yOffset > YSpawnRandomRange * 0.4f)
        {
            lane = 1;
        }
        else if (yOffset < -YSpawnRandomRange * 0.4f)
        {
            lane = -1;
        }

        std::vector<IMessageReciever*> entityToSend = { Champion };
        MessageHub::GetInstance()->SendMsg(new MsgLane(entityToSend, lane));
    }

    cachedOffsets.clear();

    for (int i = 0; i < MAX_TEAM_COUNT; i++)
    {
        int yOffset = dis(gen);
        bool closeCheck = false;

        while (!closeCheck && !cachedOffsets.empty())
        {
            closeCheck = true;
            for (int offsets : cachedOffsets)
            {
                if (abs(yOffset - offsets) <= 2) {
                    closeCheck = false;
                    yOffset = dis(gen);
                    break;
                }
            }
        }
        cachedOffsets.push_back(yOffset);

        float randomYOffset = yOffset * Map2D::GetInstance()->GetLevel()->GetTileSize().y;

        LOLEntity* Champion = new LOLEntity(UNIT);

        Champion->Team = 2;
        Champion->setPosition(glm::vec2(Team2SpawnPos.x, Team2SpawnPos.y + randomYOffset));
        Champion->spawnPos = glm::vec2(Team2SpawnPos.x, Team2SpawnPos.y + randomYOffset);
        Champion->setScale(glm::vec2(Map2D::GetInstance()->GetLevel()->GetTileSize().x, Map2D::GetInstance()->GetLevel()->GetTileSize().y));
        Champion->SetMesh(MeshBuilder::GenerateQuad("quad", Color(1, 1, 1)));
        Champion->SetTexture("Image/Minion_Blue.png");

        MessageHub::GetInstance()->AddReciever("Units", Champion);

        Champion->GetFSM()->AddState("Move", new MoveState());
        Champion->GetFSM()->AddState("InitLane", new InitLaneState());
        Champion->GetFSM()->AddState("AttackTurret", new AttackTurretState());
        Champion->GetFSM()->AddState("Attack", new AttackState());
        Champion->GetFSM()->AddState("Chase", new ChaseState());
        Champion->GetFSM()->AddState("Retreat", new RetreatState());
        Champion->GetFSM()->AddState("Damaged", new StunnedState());
        Champion->GetFSM()->AddState("Dead", new DeadState());
        Champion->GetFSM()->AddState("Respawn", new RespawnState());
        Champion->AddSkill(new ProjectileSkill({ ATTACK }, OPPOSING_TEAMS));
        Champion->AddSkill(new DashSkill({ MOVEMENT }));

        Champion->Init(GameObjectList, Map2D::GetInstance()->GetCurrentLevel(), false, false, false, false);
        Champion->InitAStar();

        int lane = 0;
        if (yOffset > YSpawnRandomRange * 0.4f)
        {
            lane = 1;
        }
        else if (yOffset < -YSpawnRandomRange * 0.4f)
        {
            lane = -1;
        }

        std::vector<IMessageReciever*> entityToSend = { Champion };
        MessageHub::GetInstance()->SendMsg(new MsgLane(entityToSend, lane));
    }

    cachedOffsets.clear();

}

int GameStateAI::HandleMessage(BaseMessage* message)
{
    if (message->GetMessageType() == GAME_OVER)
    {
        MsgGameOver* gameOver = dynamic_cast<MsgGameOver*>(message);
        if (gameOver->losingTeam == 1)
        {
            winner = "Blue";
        }
        else
        {
            winner = "Red";
        }

        pauseSim = true;

    }
    else if (message->GetMessageType() == SPAWN_ENTITY)
    {
        MsgSpawn* msgSpawn = dynamic_cast<MsgSpawn*>(message);
        if (!msgSpawn || !msgSpawn->entityToSpawn) { return -1; }
        msgSpawn->entityToSpawn->Init(GameObjectList, Map2D::GetInstance()->GetCurrentLevel(), false, false, false, false);
    }
    else if (message->GetMessageType() == MSG_DIED)
    {
        MsgDied* msgDied = dynamic_cast<MsgDied*>(message);
        if (!msgDied || !msgDied->sender) { return -1; }
        
        std::vector<IMessageReciever*> sender = { msgDied->sender };
        MessageHub::GetInstance()->SendMsg(new MsgGetRespawnTime(sender, respawnTime));
    }

    return 0;
}

void GameStateAI::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
    if (!mesh || mesh->textureID <= 0)
        return;
    
    glDisable(GL_DEPTH_TEST);
    Mtx44 ortho;
    ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
    
    RenderParameters::GetInstance()->projectionStack.PushMatrix();
    RenderParameters::GetInstance()->projectionStack.LoadMatrix(ortho);
    RenderParameters::GetInstance()->viewStack.PushMatrix();
    RenderParameters::GetInstance()->viewStack.LoadIdentity();
    RenderParameters::GetInstance()->modelStack.PushMatrix();
    RenderParameters::GetInstance()->modelStack.LoadIdentity();
    RenderParameters::GetInstance()->modelStack.Translate(x, y, 0);
    RenderParameters::GetInstance()->modelStack.Scale(size, size, size);
    glUniform1i(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_TEXT_ENABLED], 1);
    glUniform3fv(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_TEXT_COLOR], 1, &color.r);
    glUniform1i(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_LIGHTENABLED], 0);
    glUniform1i(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_COLOR_TEXTURE_ENABLED], 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh->textureID);
    glUniform1i(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_COLOR_TEXTURE], 0);
    float accum = 0;
    for (unsigned i = 0; i < text.length(); ++i)
    {
        Mtx44 characterSpacing;
        characterSpacing.SetToTranslation(accum + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
        Mtx44 MVP = RenderParameters::GetInstance()->projectionStack.Top() * RenderParameters::GetInstance()->viewStack.Top() * RenderParameters::GetInstance()->modelStack.Top() * characterSpacing;
        glUniformMatrix4fv(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_MVP], 1, GL_FALSE, &MVP.a[0]);
    
        mesh->Render((unsigned)text[i] * 6, 6);
    
        accum += (float)fontWidth[(unsigned)text[i]] / 64;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_TEXT_ENABLED], 0);
    RenderParameters::GetInstance()->modelStack.PopMatrix();
    RenderParameters::GetInstance()->viewStack.PopMatrix();
    RenderParameters::GetInstance()->projectionStack.PopMatrix();
    glEnable(GL_DEPTH_TEST);

}

void GameStateAI::CameraControls(float dt, float sensitivity)
{
    if (CMouseController::GetInstance()->IsButtonPressed(0))
    {
        camera.position.x -= CMouseController::GetInstance()->GetMouseDeltaX() * sensitivity * dt;
        camera.target.x -= CMouseController::GetInstance()->GetMouseDeltaX() * sensitivity * dt;
        camera.position.y -= CMouseController::GetInstance()->GetMouseDeltaY() * sensitivity * dt;
        camera.target.y -= CMouseController::GetInstance()->GetMouseDeltaY() * sensitivity * dt;
    }

    camera.zoomVal -= CMouseController::GetInstance()->GetMouseScrollStatus(CMouseController::GetInstance()->SCROLL_TYPE_YOFFSET) * 0.1f;
}


