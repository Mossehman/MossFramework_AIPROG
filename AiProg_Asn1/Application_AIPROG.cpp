#include "Application_AIPROG.h"
#include "GameStateAI.h"

void Application_AIPROG::OnInit()
{
	std::cout << "Init is running!" << std::endl;
	GameStateManager::GetInstance()->AddNewState("NewState", new GameStateAI());
	GameStateManager::GetInstance()->SetCurrentState("NewState");
}

void Application_AIPROG::OnRun()
{
	std::cout << "Run is running!" << std::endl;

}

void Application_AIPROG::OnExit()
{
	std::cout << "Exit called!" << std::endl;
}


