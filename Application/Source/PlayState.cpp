#include "PlayState.h"
#include "GameStateManager.h"
#include "Map2D.h"

PlayState::PlayState(Scene* newScene, std::string stateName)
{
	currentScene = newScene;
	this->gameStateName = stateName;
	nextScene = nullptr;
}

PlayState::~PlayState()
{
	Destroy();
}

bool PlayState::Init()
{
	if (!currentScene) { return false; }
	if (currentScene->Init()) { return true; }
	return false;
}

bool PlayState::Update(double dt)
{
	if (nextScene && nextScene != currentScene)
	{
		if (nextScene->Init())
		{
			currentScene->Exit();
			delete currentScene;
			currentScene = nextScene;
		}
	}

	if (!currentScene->Update(dt))
	{
		GameStateManager::GetInstance()->setCurrentState("MenuState");
		GameStateManager::GetInstance()->unPause();
		return true;
	}
	Map2D::GetInstance()->Update();
	

	return true;
}

void PlayState::Render()
{
	currentScene->Render();
}

void PlayState::Destroy()
{
	if (currentScene)
	{
		currentScene->Exit();
		delete currentScene;
		currentScene = nullptr;
	}

	if (nextScene)
	{
		nextScene->Exit();
		delete nextScene;
		nextScene = nullptr;
	}
}

void PlayState::SetActiveScene(Scene* newScene)
{
	if (!newScene) { return; }
	nextScene = newScene;
	
}
