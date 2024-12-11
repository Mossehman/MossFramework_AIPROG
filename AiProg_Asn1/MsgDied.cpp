#include "MsgDied.h"
#include "LOLEntity.h"
#include "MessageTypes.h"

MsgDied::MsgDied(std::initializer_list<std::string> TargetID, LOLEntity* me, bool deleteImmediate) : BaseMessage(TargetID, MSG_DIED, deleteImmediate)
{
	this->sender = me;
}

MsgDied::MsgDied(std::vector<IMessageReciever*> specificRecievers, LOLEntity* me, bool deleteImmediate) : BaseMessage(specificRecievers, MSG_DIED, deleteImmediate)
{
	this->sender = me;
}
