#include "Application_DungeonDelve.h"
#include "GameStateDungeon.h"

void Application_DungeonDelve::OnInit()
{
	GameStateManager::GetInstance()->AddNewState("NewState", new GameStateDungeon());
	GameStateManager::GetInstance()->SetCurrentState("NewState");
}

void Application_DungeonDelve::OnRun()
{
}

void Application_DungeonDelve::OnExit()
{
}
