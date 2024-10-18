#include "ApplicationAI.h"
#include "SceneAI.h"
#include "SceneTilemap.h"
#include "PlayState.h"
#include "MenuState.h"
#include "GameStateManager.h"

void ApplicationAI::OnInit()
{
	GameStateManager::GetInstance()->addNewState("PlayState", new PlayState(new SceneTilemap(), "PlayState"));
	GameStateManager::GetInstance()->addNewState("MenuState", new MenuState());
	GameStateManager::GetInstance()->setCurrentState("MenuState");
}

void ApplicationAI::OnRun()
{
	return;
}

void ApplicationAI::OnExit()
{
	return;
}
