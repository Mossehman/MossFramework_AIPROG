#pragma once
#include "GameState.h"
#include "Camera.h"
class MenuState: public GameState
{
public:
	virtual bool Init();
	virtual bool Update(double dt);
	virtual void Render();
	virtual void Destroy();

protected:
	float timer = 0.f;
	Camera camera;
};

