#include "TestState.h"
#include <iostream>
#include "LOLEntity.h"

void TestState::Update(float dt)
{
	std::cout << "test" << std::endl;
	stateMachine->SetCurrState("Test2");
}
