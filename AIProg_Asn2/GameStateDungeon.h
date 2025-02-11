#pragma once
#include <GameState.h>
#include "Player.h"

class GameStateDungeon : public GameState
{
public:
	virtual bool Init();
	virtual bool Update(double dt);
	virtual void Render();
	virtual void Destroy();

protected:
	Camera camera;
	glm::vec2 initialWorldMousePos;
	glm::vec2 currWorldMousePos;


	void CameraControls(float dt, float sensitivity);

	///This is for testing
	glm::vec2 startPos;
	glm::vec2 endPos;

	bool rClickPressed = false;
	bool lClickPressed = false;

	Player* player;
	GameObject* go;

	std::vector<glm::vec2> waypointList;
	void RenderPath(Color pathColor);
};

