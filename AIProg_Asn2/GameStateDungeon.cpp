#include "GameStateDungeon.h"
#include <Map2D.h>
#include "MazeMap.h"
#include <sstream>

#include <MessageHub.h>
#include "TurnManager.h"
#include "EnemyManager.h"
#include <KeyboardController.h>
#include "ScaredEnemy.h"


#include <MouseController.h>

bool GameStateDungeon::Init()
{
	RenderParameters::GetInstance()->Init(glm::vec4(0.f, 0.f, 0.3f, 0.8f), "Shader//comg.vertexshader", "Shader//comg.fragmentshader");
	camera.Init(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	camera.ToClamp = false;
	textMesh->textureID = LoadTGA("Image//calibri.tga");

	MazeMap* map = new MazeMap(30, 30, glm::vec2(15, 15), nullptr, new PerlinNoiseConfig(5, 0.2));


	map->SetTileMesh(MeshBuilder::GenerateQuad("tile", Color(1, 0, 0)));
	map->Init();

	player = new Player();
	player->setPosition(glm::vec2(0, 0));
	player->setScale(glm::vec2(15, 15));
	player->SetMesh(MeshBuilder::GenerateQuad("tile", Color(1, 1, 1)));
	player->SetTexture("Image/DD_player_1.png");
	player->movementRange = 5;

	std::vector<glm::vec2> spawnableTiles = map->GetTilesWithID(2);

	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (i <= 6)
		{
			enemies[i] = new Enemy();
			enemies[i]->setScale(glm::vec2(10, 10));
			enemies[i]->SetMesh(MeshBuilder::GenerateQuad("tile", Color(1, 1, 1)));
			enemies[i]->visionRange = 5;
			enemies[i]->SetTexture("Image/DD_chaser.png");
		}
		else
		{
			enemies[i] = new ScaredEnemy();
			enemies[i]->setScale(glm::vec2(10, 10));
			enemies[i]->SetMesh(MeshBuilder::GenerateQuad("tile", Color(1, 1, 1)));
			enemies[i]->movementRange = 5;
			enemies[i]->visionRange = 2;
			enemies[i]->SetTexture("Image/DD_lurker.png");
		}
		int randomIndex = std::rand() % spawnableTiles.size();
		glm::vec2 randomTile = spawnableTiles[randomIndex];
		enemies[i]->setPosition(randomTile);

		enemies[i]->destination = enemies[i]->getPosition();
		MessageHub::GetInstance()->AddReciever("Enemy", enemies[i]);
		EnemyManager::GetInstance()->enemies.push_back(enemies[i]);
	}

	go = new GameObject();
	go->setPosition(glm::vec2(1000000, 100000));
	go->setScale(glm::vec2(10, 10));
	go->SetMesh(MeshBuilder::GenerateQuad("tile", Color(0, 1, 1)));


	Map2D::GetInstance()->AddLevel(0, map);
	Map2D::GetInstance()->ChangeLevel(0);
	map->InitialiseSpawn(player->getPosition(), 0.3f);

	EnemyManager::GetInstance()->enemyPlayer = player;

	initialWorldMousePos = CMouseController::GetInstance()->MouseToWorldPos(RenderParameters::GetInstance()->getWindowSizeX(), RenderParameters::GetInstance()->getWindowSizeY(), camera);

	std::srand(std::time(nullptr));

	TurnManager::GetInstance()->RegisterTurnID(0, "Player");
	TurnManager::GetInstance()->RegisterTurnID(1, "Enemy");
	MessageHub::GetInstance()->AddReciever("Player", player);

	TurnManager::GetInstance()->NextTurn();

	MessageHub::GetInstance()->AddReciever("Scene", this);
	return true;
}

bool GameStateDungeon::Update(double dt)
{
	currWorldMousePos = CMouseController::GetInstance()->MouseToWorldPos(RenderParameters::GetInstance()->getWindowSizeX(), RenderParameters::GetInstance()->getWindowSizeY(), camera);
	CameraControls(dt, 20.0f);
	camera.Update(dt);
	initialWorldMousePos = currWorldMousePos;
	
	if (gameOver) { return true; }
	if (CMouseController::GetInstance()->IsButtonDown(1) && !rClickPressed && player->isTurn)
	{
		glm::vec2 tileSize = Map2D::GetInstance()->GetLevel()->GetTileSize();
		glm::ivec2 mouseTilePos = Map2D::GetInstance()->PosToTilePos(currWorldMousePos + (glm::vec2(tileSize.x, -tileSize.y) * 0.5f));
		go->setPosition(Map2D::GetInstance()->TileToPos(mouseTilePos));
		if (player->walkableAreas.count(mouseTilePos) > 0)
		{
			player->OnMovementStart(currWorldMousePos);
		}

		rClickPressed = true;
	}
	else if (!CMouseController::GetInstance()->IsButtonDown(1) && rClickPressed)
	{
		rClickPressed = false;
	}

	EnemyManager::GetInstance()->Update(dt);
	player->Update(dt);
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		enemies[i]->Update(dt);
	}


	MessageHub::GetInstance()->Update();
	FPS =  1 / dt;

	return true;
}

void GameStateDungeon::Render()
{
	std::ostringstream ss; 
	RenderParameters::GetInstance()->Render(camera, 1200, 800);

	glDepthMask(GL_FALSE);
	Map2D::GetInstance()->Render(camera.position, glm::vec2(100, 100));
	RenderPath(Color(1, 0, 0));
	
	player->RenderPath(Color(0, 0, 1));
	player->RenderTiles();
	EnemyManager::GetInstance()->Render();
	go->Render();
	for (int i = 0; i < GameObjectList.size(); i++)
	{
		GameObjectList[i]->Render();
	}

	player->Render();
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		enemies[i]->Render();
	}

	glDepthMask(GL_TRUE);

	ss.str("");
	ss << "Health: " << player->health;
	RenderTextOnScreen(textMesh, ss.str(), Color(0, 1, 0), 3, 50, 22);

	ss.str("");
	std::string turnOrder = "";
	if (TurnManager::GetInstance()->turnOrder == 0) {
		turnOrder = "Enemies";
	}
	else
	{
		turnOrder = "Player";
	}
		
	RenderTextOnScreen(textMesh, winner, Color(0, 1, 0), 3, 50, 26);

	ss << "Turn: " << turnOrder;
	RenderTextOnScreen(textMesh, ss.str(), Color(0, 1, 0), 3, 50, 18);

	ss.str("");
	ss << "Current Event: " << TurnManager::GetInstance()->currentEvent;
	RenderTextOnScreen(textMesh, ss.str(), Color(0, 1, 0), 3, 50, 14);

	ss.str("");
	int tileCost = 0;
	glm::vec2 tileSize = Map2D::GetInstance()->GetLevel()->GetTileSize();
	glm::ivec2 mouseTilePos = Map2D::GetInstance()->PosToTilePos(currWorldMousePos + (glm::vec2(tileSize.x, -tileSize.y) * 0.5f));

	tileCost = (std::max)(1, Map2D::GetInstance()->GetLevel()->GetTilemap()[mouseTilePos.y][mouseTilePos.x]->cost - 1);

	ss << "Tile Movement Cost: " << tileCost;
	RenderTextOnScreen(textMesh, ss.str(), Color(0, 1, 0), 3, 50, 10);

	ss.str("");
	ss.precision(2);
	ss << "FPS:" << FPS;
	RenderTextOnScreen(textMesh, ss.str(), Color(0, 1, 0), 3, 50, 6);
}

void GameStateDungeon::Destroy()
{
	delete go;
	delete player;

	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		delete enemies[i];
	}
}

void GameStateDungeon::CameraControls(float dt, float sensitivity)
{
	glm::vec2 mouseDelta = currWorldMousePos - initialWorldMousePos;

	if (CMouseController::GetInstance()->IsButtonPressed(0))
	{
		camera.position.x -= mouseDelta.x * sensitivity * dt;
		camera.target.x -= mouseDelta.x * sensitivity * dt;
		camera.position.y -= mouseDelta.y * sensitivity * dt;
		camera.target.y -= mouseDelta.y * sensitivity * dt;
	}

	camera.zoomVal -= CMouseController::GetInstance()->GetMouseScrollStatus(CMouseController::GetInstance()->SCROLL_TYPE_YOFFSET) * 0.1f;
}

void GameStateDungeon::RenderPath(Color pathColor)
{
	if (waypointList.empty()) { return; }

	for (int i = 0; i < waypointList.size(); i++)
	{
		if (i == waypointList.size() - 1) { break; }

		GameObject* lineObj = new GameObject();
		lineObj->SetMesh(MeshBuilder::GenerateLineDir("Line", pathColor, waypointList[i], waypointList[i + 1]));
		lineObj->Render();
		delete lineObj;
	}
}

void GameStateDungeon::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

int GameStateDungeon::HandleMessage(BaseMessage* message)
{
	if (!message) { return -1; }

	if (message->GetMessageType() == 1)
	{
		this->winner = "You Win!";
		gameOver = true;
	}
	else if (message->GetMessageType() == 2)
	{
		gameOver = true;
		this->winner = "You Died!";
	}

	return 0;
}

