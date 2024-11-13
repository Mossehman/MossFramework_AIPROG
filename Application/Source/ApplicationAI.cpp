#include "ApplicationAI.h"
#include "SceneAI.h"
#include "SceneTilemap.h"
#include "PlayState.h"
#include "MenuState.h"
#include "GameStateManager.h"

void ApplicationAI::OnInit()
{
	//GameStateManager::GetInstance()->AddNewState("PlayState", new PlayState(new SceneTilemap(), "PlayState"));
	//GameStateManager::GetInstance()->AddNewState("MenuState", new MenuState());
	//GameStateManager::GetInstance()->SetCurrentState("MenuState");
}

void ApplicationAI::OnRun()
{
	return;
}

void ApplicationAI::OnExit()
{
	return;
}
