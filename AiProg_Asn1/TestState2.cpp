#include "TestState2.h"
#include <iostream>
#include "LOLEntity.h"

void TestState2::Update(float dt)
{
	std::cout << "test2" << std::endl;
	stateMachine->SetCurrState("Test");

}
