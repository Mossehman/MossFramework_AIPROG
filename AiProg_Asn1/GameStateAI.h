#pragma once
#include <GameState.h>
#include <Camera.h>
#include <GameObject.h>

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

	float timer = 0.f;
	Camera camera;
};

