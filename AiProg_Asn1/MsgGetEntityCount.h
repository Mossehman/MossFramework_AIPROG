#pragma once
#include "BaseMessage.h"
#include "EntityTypes.h"

class MsgGetEntityCount : BaseMessage
{
public:
	ENTITY_CLASSES entityClass;
	int team = -1;

	MsgGetEntityCount(std::initializer_list<std::string> TargetID, ENTITY_CLASSES entityClass, int team, bool deleteImmediate = true);
	MsgGetEntityCount(std::vector<IMessageReciever*> specificRecievers, ENTITY_CLASSES entityClass, int team, bool deleteImmediate = true);
};

