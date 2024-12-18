#pragma once
#include <GameState.h>
#include <Camera.h>
#include <GameObject.h>
#include <EntityAI2D.h>
#include <FiniteState.h>
#include "LOLEntity.h"
#include <MeshBuilder.h>

#include <Map2D.h>

#include "LevelsAI.h"
#include "MessageHub.h"

#define INHIBITOR_INDEX 3

#define TEAM_ONE_TURRET_INDEX_T 4
#define TEAM_ONE_TURRET_INDEX_M 5
#define TEAM_ONE_TURRET_INDEX_B 6


#define TEAM_TWO_TURRET_INDEX_T 7
#define TEAM_TWO_TURRET_INDEX_M 8
#define TEAM_TWO_TURRET_INDEX_B 9

#define MAX_TEAM_COUNT 5

#define TIME_BEFORE_RESPAWN_INCREASE 120

static const int fontWidth[] = { 0,26,26,26,26,26,26,26,26,26,26,26,26,0,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,12,17,21,26,26,37,35,11,16,16,26,26,13,16,13,20,26,26,26,26,26,26,26,26,26,26,14,14,26,26,26,24,46,30,28,28,32,25,24,33,32,13,17,27,22,44,34,34,27,35,28,24,25,33,30,46,27,25,24,16,20,16,26,26,15,25,27,22,27,26,16,24,27,12,12,24,12,42,27,27,27,27,18,20,17,27,23,37,23,24,21,16,24,16,26,26,26,26,13,16,22,36,26,26,21,54,24,18,45,26,24,26,26,13,13,22,22,26,26,47,23,37,20,18,44,26,21,25,12,17,26,26,26,26,26,26,20,43,21,27,26,16,26,20,18,26,17,17,15,29,30,13,16,13,22,27,33,35,35,24,30,30,30,30,30,30,40,28,25,25,25,25,13,13,13,13,32,34,34,34,34,34,34,26,35,33,33,33,33,25,27,27,25,25,25,25,25,25,40,22,26,26,26,26,12,12,12,12,27,27,27,27,27,27,27,26,28,27,27,27,27,24,27,24 };

class GameStateAI : public GameState
{
public:
	virtual bool Init();
	virtual bool Update(double dt);
	virtual void Render();
	virtual void Destroy();

protected:

	float FPS = 0.0f;
	int NumBlue = 0;
	int NumRed = 0;

	Mesh* textMesh = MeshBuilder::GenerateText("text", 16, 16);

	float timeElapsed = 0.0f;
	float respawnTimer = 0.0f;
	float respawnTime = 5.0f;
	std::string winner = "";

	glm::vec2 Team1SpawnPos;
	glm::vec2 Team2SpawnPos;
	Camera camera;

	bool pauseSim = true;
	
	void CameraControls(float dt, float sensitivity = 1000.0f);

	void GenerateTurrets(int tileID, std::string laneID, int team);
	void TurretsInit();
	void EntitiesInit();

	int YSpawnRandomRange = 14;

	virtual int HandleMessage(BaseMessage* message) override;

	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);

};

