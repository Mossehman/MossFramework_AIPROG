#include "TetherSkill.h"
#include "LOLEntity.h"
#include <iostream>

void TetherSkill::WhileAttacking(float dt)
{
	this->championEntity->setPosition(tetherPos);
	this->championEntity->SolveAStar();

	EndSkill();
}

void TetherSkill::OnSetup()
{
	//generate the tether position
	tetherPos = glm::vec2(this->championEntity->getPosition().x, this->championEntity->getPosition().y);
	std::cout << "Teleport setup!" << std::endl;
}
