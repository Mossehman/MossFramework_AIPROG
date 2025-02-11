#include "Application_DungeonDelve.h"
#include "GameStateDungeon.h"
#include "GameStateWin.h"

void Application_DungeonDelve::OnInit()
{
	GameStateManager::GetInstance()->AddNewState("GameState", new GameStateDungeon());
	GameStateManager::GetInstance()->AddNewState("WinState", new GameStateWin());
	GameStateManager::GetInstance()->SetCurrentState("GameState");
}

void Application_DungeonDelve::OnRun()
{
}

void Application_DungeonDelve::OnExit()
{
}
