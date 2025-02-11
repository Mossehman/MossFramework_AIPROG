#include "GameStateDungeon.h"
#include <Map2D.h>
#include "MazeMap.h"

#include <MouseController.h>

bool GameStateDungeon::Init()
{
	RenderParameters::GetInstance()->Init(glm::vec4(0.f, 0.f, 0.3f, 0.8f), "Shader//comg.vertexshader", "Shader//comg.fragmentshader");
	camera.Init(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	camera.ToClamp = false;
	MazeMap* map = new MazeMap(60, 60, glm::vec2(15, 15), nullptr, new PerlinNoiseConfig(5, 0.2));

	map->SetTileMesh(MeshBuilder::GenerateQuad("tile", Color(1, 0, 0)));
	map->Init();

	player = new Player();
	player->setPosition(glm::vec2(300, -300));
	player->setScale(glm::vec2(10, 10));
	player->SetMesh(MeshBuilder::GenerateQuad("tile", Color(1, 0, 0)));

	go = new GameObject();
	go->setPosition(glm::vec2(300, -300));
	go->setScale(glm::vec2(10, 10));
	go->SetMesh(MeshBuilder::GenerateQuad("tile", Color(0, 1, 1)));


	Map2D::GetInstance()->AddLevel(0, map);
	Map2D::GetInstance()->ChangeLevel(0);
	map->InitialiseSpawn(player->getPosition(), 0.3f);

	initialWorldMousePos = CMouseController::GetInstance()->MouseToWorldPos(RenderParameters::GetInstance()->getWindowSizeX(), RenderParameters::GetInstance()->getWindowSizeY(), camera);


	player->OnTurnStart();

	return true;
}

bool GameStateDungeon::Update(double dt)
{
	currWorldMousePos = CMouseController::GetInstance()->MouseToWorldPos(RenderParameters::GetInstance()->getWindowSizeX(), RenderParameters::GetInstance()->getWindowSizeY(), camera);
	CameraControls(dt, 20.0f);
	camera.Update(dt);
	initialWorldMousePos = currWorldMousePos;
	
	if (CMouseController::GetInstance()->IsButtonDown(1) && !rClickPressed)
	{
		glm::vec2 tileSize = Map2D::GetInstance()->GetLevel()->GetTileSize();
		glm::ivec2 mouseTilePos = Map2D::GetInstance()->PosToTilePos(currWorldMousePos + (glm::vec2(tileSize.x, -tileSize.y) * 0.5f));
		go->setPosition(Map2D::GetInstance()->TileToPos(mouseTilePos));
		if (player->walkableAreas.count(mouseTilePos) > 0)
		{
			player->OnMovementStart(currWorldMousePos + (glm::vec2(tileSize.x, -tileSize.y) * 0.5f));
		}

		rClickPressed = true;
	}
	else if (!CMouseController::GetInstance()->IsButtonDown(1) && rClickPressed)
	{
		rClickPressed = false;
	}

	player->Update(dt);

	std::cout << 1 / dt << std::endl;

	return true;
}

void GameStateDungeon::Render()
{
	RenderParameters::GetInstance()->Render(camera, 1200, 800);

	glDepthMask(GL_FALSE);
	Map2D::GetInstance()->Render(camera.position, glm::vec2(100, 100));
	RenderPath(Color(1, 0, 0));
	
	player->RenderPath(Color(0, 0, 1));
	player->RenderTiles();
	go->Render();
	for (int i = 0; i < GameObjectList.size(); i++)
	{
		GameObjectList[i]->Render();
	}

	player->Render();

	glDepthMask(GL_TRUE);
}

void GameStateDungeon::Destroy()
{
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
