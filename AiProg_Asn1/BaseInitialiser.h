#pragma once
#include "LOLEntity.h"
#include "FiniteState.h"

#include "MoveState.h"
#include "AttackState.h"

class BaseInitialiser
{
public:
	virtual void Initialise(LOLEntity*& championEntity) = 0;
};

