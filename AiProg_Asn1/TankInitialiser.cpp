#include "TankInitialiser.h"

void TankInitialiser::Initialise(LOLEntity*& championEntity)
{
	championEntity->GetFSM()->AddState("Move", new MoveState());
}
