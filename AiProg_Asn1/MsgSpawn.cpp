#include "MsgSpawn.h"
#include "MessageTypes.h"

MsgSpawn::MsgSpawn(std::initializer_list<std::string> TargetID, LOLEntity* entityToSpawn, bool deleteImmediate) : BaseMessage(TargetID, SPAWN_ENTITY, deleteImmediate)
{
	this->entityToSpawn = entityToSpawn;
}

MsgSpawn::MsgSpawn(std::vector<IMessageReciever*> specificRecievers, LOLEntity* entityToSpawn, bool deleteImmediate) : BaseMessage(specificRecievers, SPAWN_ENTITY, deleteImmediate)
{
	this->entityToSpawn = entityToSpawn;
}
