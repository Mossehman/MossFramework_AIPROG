#pragma once
#include "BaseInitialiser.h"

class TankInitialiser : public BaseInitialiser
{
public:
	virtual void Initialise(LOLEntity*& championEntity) override;
};

