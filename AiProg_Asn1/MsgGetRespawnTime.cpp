#include "MsgGetRespawnTime.h"
#include "MessageTypes.h"

MsgGetRespawnTime::MsgGetRespawnTime(std::initializer_list<std::string> TargetID, float respawnTime, bool deleteImmediate) : BaseMessage(TargetID, MSG_GETRESPAWNTIME, deleteImmediate)
{
	this->respawnTime = respawnTime;
}

MsgGetRespawnTime::MsgGetRespawnTime(std::vector<IMessageReciever*> specificRecievers, float respawnTime, bool deleteImmediate) : BaseMessage(specificRecievers, MSG_GETRESPAWNTIME, deleteImmediate)
{
	this->respawnTime = respawnTime;
}
