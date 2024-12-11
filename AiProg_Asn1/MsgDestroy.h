#pragma once
#include "BaseMessage.h"

class MsgDestroy : public BaseMessage
{
public:
	MsgDestroy(std::initializer_list<std::string> TargetID, bool deleteImmediate = true);
	MsgDestroy(std::vector<IMessageReciever*> specificRecievers, bool deleteImmediate = true);
};

