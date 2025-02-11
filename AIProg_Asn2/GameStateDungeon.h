#pragma once
#include <GameState.h>
#include "Player.h"
#include "Enemy.h"

#define MAX_ENEMIES 10

class GameStateDungeon : public GameState
{
public:
	virtual bool Init();
	virtual bool Update(double dt);
	virtual void Render();
	virtual void Destroy();

protected:
	bool gameOver = false;
	std::string winner = "";
	Mesh* textMesh = MeshBuilder::GenerateText("text", 16, 16);
	Camera camera;
	glm::vec2 initialWorldMousePos;
	glm::vec2 currWorldMousePos;


	void CameraControls(float dt, float sensitivity);

	///This is for testing
	glm::vec2 startPos;
	glm::vec2 endPos;

	bool rClickPressed = false;

	Player* player;
	GameObject* go;

	float FPS;

	Enemy* enemies[MAX_ENEMIES];

	std::vector<glm::vec2> waypointList;
	void RenderPath(Color pathColor);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	virtual int HandleMessage(BaseMessage* message) override;
};

