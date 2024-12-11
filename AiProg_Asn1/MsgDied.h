#pragma once
#include "BaseMessage.h"

class LOLEntity;

class MsgDied : public BaseMessage
{

public:
	LOLEntity* sender = nullptr;

	MsgDied(std::initializer_list<std::string> TargetID, LOLEntity* me, bool deleteImmediate = true);
	MsgDied(std::vector<IMessageReciever*> specificRecievers, LOLEntity* me, bool deleteImmediate = true);
};

