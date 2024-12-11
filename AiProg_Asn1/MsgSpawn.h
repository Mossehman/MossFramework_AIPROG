#pragma once
#include "BaseMessage.h"

class LOLEntity;

class MsgSpawn : public BaseMessage
{
public:
	LOLEntity* entityToSpawn;

	MsgSpawn(std::initializer_list<std::string> TargetID, LOLEntity* entityToSpawn, bool deleteImmediate = true);
	MsgSpawn(std::vector<IMessageReciever*> specificRecievers, LOLEntity* entityToSpawn, bool deleteImmediate = true);
};

