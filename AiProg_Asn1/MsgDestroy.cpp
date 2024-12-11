#include "MsgDestroy.h"
#include "MessageTypes.h"

MsgDestroy::MsgDestroy(std::initializer_list<std::string> TargetID, bool deleteImmediate) : BaseMessage(TargetID, DESTROY_ENTITY, deleteImmediate)
{
}

MsgDestroy::MsgDestroy(std::vector<IMessageReciever*> specificRecievers, bool deleteImmediate) : BaseMessage(specificRecievers, DESTROY_ENTITY, deleteImmediate)
{
}
