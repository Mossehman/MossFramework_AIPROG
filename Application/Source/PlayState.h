#pragma once
#include "GameState.h"
#include "Scene.h"
class PlayState: public GameState
{
public:
	PlayState(Scene* newScene, std::string stateName);
	~PlayState();

	virtual bool Init();
	virtual bool Update(double dt);
	virtual void Render();
	virtual void Destroy();

	void SetActiveScene(Scene* newScene);

protected:
	Scene* currentScene;
	Scene* nextScene;
};

