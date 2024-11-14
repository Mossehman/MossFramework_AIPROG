#pragma once
#include <GameState.h>
#include <Camera.h>
#include <GameObject.h>
#include <EntityAI2D.h>
#include <FiniteState.h>

#include <Map2D.h>

#include "LevelsAI.h"

class GameStateAI : public GameState
{
public:
	virtual bool Init();
	virtual bool Update(double dt);
	virtual void Render();
	virtual void Destroy();

protected:
	GameObject* go1;
	GameObject* go2;
	EntityAI2D* newEntity;
	EntityAI2D* newEntity2;

	float timer = 0.f;
	Camera camera;
};

