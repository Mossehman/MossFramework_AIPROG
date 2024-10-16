#include "SceneTilemap.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "KeyboardController.h"
#include "GUImanager.h"
#include "InventoryManager.h"
#include "BGMManager.h"

#define WIDTH 1200
#define HEIGHT 800

bool SceneTilemap::Init()
{
	RenderParameters::GetInstance()->Init(glm::vec4(0, 0, 1, 0), "Shader//comg.vertexshader", "Shader//comg.fragmentshader");
	camera.Init(Vector3(300, -300, 1), Vector3(300, -300, 0), Vector3(0, 1, 0));

	//setup map
	Map2D::GetInstance()->setTileMesh(MeshBuilder::GenerateQuad("Quad", Color(1.f, 1.f, 1.f), 1.0f));
	Map2D::GetInstance()->setTileSize(glm::vec2(50, 50));
	Map2D::GetInstance()->mapSizeX[LevelIDs::SUPERDESTROYER] = 200; 
	Map2D::GetInstance()->mapSizeY[LevelIDs::SUPERDESTROYER] = 200; 
	Map2D::GetInstance()->setTileMesh(MeshBuilder::GenerateQuad("Quad", Color(1, 0, 1)));
	Map2D::GetInstance()->generatePerlinMap(LevelIDs::SUPERDESTROYER);
	//Map2D::GetInstance()->writeMapFromCSV("Maps/tilemap1.csv", LevelIDs::SUPERDESTROYER);
	Map2D::GetInstance()->Init(LevelIDs::SUPERDESTROYER);

	minimap = new Minimap;
	minimap->Init();

	entity = new Entity2D();
	entity->setMesh(MeshBuilder::GenerateQuad("quad", Color(1, 1, 1)));
	entity->setTexture("Image/grass.tga");
	entity->setPosition(glm::vec2(100, -70));
	entity->setScale(glm::vec2(30, 100));
	entity->setHitboxOffset(glm::vec2(0, 0));
	entity->setHitboxSize(glm::vec2(30, 100));
	entity->gravityValue = glm::vec2(0, -10);
	entity->Init(LevelIDs::SUPERDESTROYER, true, true, false, false);


	camera.minPos.x = WIDTH * 0.5f - (Map2D::GetInstance()->getTileSize().x * 0.5f);
	camera.maxPos.y = (HEIGHT * 0.5f - (Map2D::GetInstance()->getTileSize().y * 0.5f)) * -1;

	camera.maxPos.x = (Map2D::GetInstance()->mapSizeX[Map2D::GetInstance()->getCurrentLevel()] - 1) * Map2D::GetInstance()->getTileSize().x + (Map2D::GetInstance()->getTileSize().x * 0.5f) - (WIDTH * 0.5f);
	camera.minPos.y = ((Map2D::GetInstance()->mapSizeY[Map2D::GetInstance()->getCurrentLevel()] - 1) * Map2D::GetInstance()->getTileSize().y + (Map2D::GetInstance()->getTileSize().y * 0.5f) - (HEIGHT * 0.5f)) * -1;


	std::cout << camera.maxPos.x << ", " << camera.minPos.y << std::endl;
	GUImanager::GetInstance()->Init();

	return true;
}

bool SceneTilemap::Update(double dt)
{
	//BGMManager::GetInstance()->playBGM();
	//BGMManager::GetInstance()->Update(dt);

	mouseWorldPos = CMouseController::GetInstance()->MouseToWorldPos(WIDTH, HEIGHT, glm::vec2(camera.position.x, camera.position.y));
	
	camera.position = Vector3(entity->getPosition().x, entity->getPosition().y, 1.f);
	camera.target = Vector3(entity->getPosition().x, entity->getPosition().y, 0.f);

	camera.Update(dt);

	entity->debugMove(10000, dt);
	entity->Update(dt);

	GUImanager::GetInstance()->Update(dt);
	CMouseController::GetInstance()->PostUpdate();

	minimap->Update(dt, entity->getPosition());

	if (CKeyboardController::GetInstance()->IsKeyPressed('H'))
	{
		BGMManager::GetInstance()->skipSong();
	}

	//std::cout << entity->getPosition().x << ", " << entity->getPosition().y << std::endl;
	return true;


}

void SceneTilemap::Render()
{
	RenderParameters::GetInstance()->Render(camera, WIDTH, HEIGHT);
	
	minimap->Render(camera);
	entity->Render();
	Map2D::GetInstance()->Render(glm::vec2(camera.position.x, camera.position.y), glm::vec2(15, 10));
	GUImanager::GetInstance()->Render();

}

void SceneTilemap::Exit()
{
	delete entity;
	delete minimap;
	
}
