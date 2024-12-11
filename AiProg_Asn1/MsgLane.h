#pragma once
#include <BaseMessage.h>

class MsgLane : public BaseMessage
{
public:
	int lane = 0;
	MsgLane(std::initializer_list<std::string> TargetID, int lane, bool deleteImmediate = true);
	MsgLane(std::vector<IMessageReciever*> specificRecievers, int lane, bool deleteImmediate = true);
};

