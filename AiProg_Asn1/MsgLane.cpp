#include "MsgLane.h"
#include "MessageTypes.h"

MsgLane::MsgLane(std::initializer_list<std::string> TargetID, int lane, bool deleteImmediate) : BaseMessage(TargetID, GET_LANE, deleteImmediate)
{
	this->lane = lane;
}

MsgLane::MsgLane(std::vector<IMessageReciever*> specificRecievers, int lane, bool deleteImmediate) : BaseMessage(specificRecievers, GET_LANE, deleteImmediate)
{
	this->lane = lane;
}
