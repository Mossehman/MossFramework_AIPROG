#include "MsgGetEntityCount.h"
#include "MessageTypes.h"

MsgGetEntityCount::MsgGetEntityCount(std::initializer_list<std::string> TargetID, ENTITY_CLASSES entityClass, int team, bool deleteImmediate) : BaseMessage(TargetID, MSG_GETENTITYCOUNT, deleteImmediate)
{
	this->entityClass = entityClass;
	this->team = team;
}

MsgGetEntityCount::MsgGetEntityCount(std::vector<IMessageReciever*> specificRecievers, ENTITY_CLASSES entityClass, int team, bool deleteImmediate) : BaseMessage(specificRecievers, MSG_GETENTITYCOUNT, deleteImmediate)
{
	this->entityClass = entityClass;
	this->team = team;
}
